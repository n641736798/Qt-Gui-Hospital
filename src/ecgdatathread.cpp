#include "ecgdatathread.h"
#include <QDebug>
#include <QRandomGenerator>

ECGDataThread::ECGDataThread(QObject *parent)
    : QThread(parent)
    , running(false)
    , paused(false)
    , heartRate(72)        // 默认心率72 BPM
    , amplitude(1.0)       // 默认振幅
    , noiseLevel(0.05)     // 默认噪声水平
    , timeStep(1.0 / SAMPLE_RATE)  // 采样间隔
    , currentTime(0.0)
{
}

ECGDataThread::~ECGDataThread()
{
    stopGeneration();
    wait(); // 等待线程结束
}

void ECGDataThread::startGeneration()
{
    QMutexLocker locker(&mutex);
    if (!running) {
        running = true;
        paused = false;
        currentTime = 0.0;
        start(); // 启动线程
    }
}

void ECGDataThread::stopGeneration()
{
    QMutexLocker locker(&mutex);
    running = false;
    condition.wakeAll();
}

void ECGDataThread::pauseGeneration()
{
    QMutexLocker locker(&mutex);
    paused = true;
}

void ECGDataThread::resumeGeneration()
{
    QMutexLocker locker(&mutex);
    paused = false;
    condition.wakeAll();
}

void ECGDataThread::setHeartRate(int bpm)
{
    QMutexLocker locker(&mutex);
    heartRate = qBound(40, bpm, 200); // 限制心率范围
}

void ECGDataThread::setAmplitude(double amp)
{
    QMutexLocker locker(&mutex);
    amplitude = qBound(0.1, amp, 5.0); // 限制振幅范围
}

void ECGDataThread::setNoiseLevel(double noise)
{
    QMutexLocker locker(&mutex);
    noiseLevel = qBound(0.0, noise, 0.5); // 限制噪声范围
}

void ECGDataThread::run()
{
    while (true) {
        mutex.lock();
        
        // 检查是否需要停止
        if (!running) {
            mutex.unlock();
            break;
        }
        
        // 检查是否暂停
        if (paused) {
            condition.wait(&mutex);
            mutex.unlock();
            continue;
        }
        
        mutex.unlock();
        
        // 生成ECG数据
        generateECGData();
        
        // 控制更新频率
        msleep(UPDATE_INTERVAL);
    }
}

void ECGDataThread::generateECGData()
{
    ECGDataPoint dataPoint;
    dataPoint.timestamp = currentTime;
    
    // 为4个导联生成数据
    for (int i = 0; i < 4; ++i) {
        dataPoint.values[i] = generateECGValue(currentTime, i);
    }
    
    // 发送数据
    emit dataReady(dataPoint);
    
    // 更新时间
    currentTime += timeStep * (UPDATE_INTERVAL / 2.0); // 调整时间步长以匹配更新频率
}

double ECGDataThread::generateECGValue(double t, int channel)
{
    // 基础心电波形生成
    double heartBeat = generateHeartBeat(t);
    
    // 为不同导联添加相位偏移和幅度调整
    double phaseOffset = channel * 0.1;
    double amplitudeScale = 1.0;
    
    switch (channel) {
    case 0: // 导联 I
        amplitudeScale = 1.0;
        break;
    case 1: // 导联 II  
        amplitudeScale = 1.2;
        phaseOffset += 0.05;
        break;
    case 2: // 导联 III
        amplitudeScale = 0.8;
        phaseOffset += 0.1;
        break;
    case 3: // 导联 aVR
        amplitudeScale = -0.6; // aVR通常是倒置的
        phaseOffset += 0.15;
        break;
    }
    
    double value = heartBeat * amplitudeScale * amplitude;
    
    // 添加基线漂移
    double baseline = 0.1 * sin(2 * M_PI * 0.1 * t);
    
    // 添加随机噪声
    double noise = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0 * noiseLevel;
    
    return value + baseline + noise + phaseOffset;
}

double ECGDataThread::generateHeartBeat(double t)
{
    // 心率转换为频率 (Hz)
    double frequency = heartRate / 60.0;
    
    // 计算心跳周期内的相对时间
    double cycleTime = fmod(t * frequency, 1.0);
    
    // 生成典型的心电图波形 (PQRST波)
    double ecgValue = 0.0;
    
    if (cycleTime < 0.1) {
        // P波 (心房去极化)
        double pTime = cycleTime / 0.1;
        ecgValue = 0.2 * sin(M_PI * pTime);
    }
    else if (cycleTime < 0.2) {
        // PR段 (等电位线)
        ecgValue = 0.0;
    }
    else if (cycleTime < 0.25) {
        // Q波 (小的负向波)
        double qTime = (cycleTime - 0.2) / 0.05;
        ecgValue = -0.1 * sin(M_PI * qTime);
    }
    else if (cycleTime < 0.35) {
        // R波 (主要的正向波)
        double rTime = (cycleTime - 0.25) / 0.1;
        ecgValue = 1.0 * sin(M_PI * rTime);
    }
    else if (cycleTime < 0.45) {
        // S波 (负向波)
        double sTime = (cycleTime - 0.35) / 0.1;
        ecgValue = -0.3 * sin(M_PI * sTime);
    }
    else if (cycleTime < 0.55) {
        // ST段 (等电位线)
        ecgValue = 0.0;
    }
    else if (cycleTime < 0.75) {
        // T波 (心室复极化)
        double tTime = (cycleTime - 0.55) / 0.2;
        ecgValue = 0.3 * sin(M_PI * tTime);
    }
    else {
        // 等电位线
        ecgValue = 0.0;
    }
    
    return ecgValue;
}