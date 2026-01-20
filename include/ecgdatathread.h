#ifndef ECGDATATHREAD_H
#define ECGDATATHREAD_H

#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>
#include <cmath>

struct ECGDataPoint {
    double timestamp;
    double values[4]; // 4个导联的数据
};

class ECGDataThread : public QThread
{
    Q_OBJECT

public:
    explicit ECGDataThread(QObject *parent = nullptr);
    ~ECGDataThread();

    void startGeneration();
    void stopGeneration();
    void pauseGeneration();
    void resumeGeneration();
    
    void setHeartRate(int bpm); // 设置心率
    void setAmplitude(double amplitude); // 设置振幅
    void setNoiseLevel(double noise); // 设置噪声水平

signals:
    void dataReady(const ECGDataPoint &data);

protected:
    void run() override;

private:
    void generateECGData();
    double generateECGValue(double t, int channel);
    double generateHeartBeat(double t);
    
    QMutex mutex;
    QWaitCondition condition;
    
    bool running;
    bool paused;
    
    // ECG参数
    int heartRate;        // 心率 (BPM)
    double amplitude;     // 振幅
    double noiseLevel;    // 噪声水平
    double timeStep;      // 时间步长
    double currentTime;   // 当前时间
    
    // 采样参数
    static const int SAMPLE_RATE = 25; // 降低到25Hz采样率，减少数据量
    static const int UPDATE_INTERVAL = 40; // 40ms更新间隔，降低更新频率
};

#endif // ECGDATATHREAD_H