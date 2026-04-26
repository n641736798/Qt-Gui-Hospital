#include "documentscannerdialog.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSlider>
#include <QComboBox>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

DocumentScannerDialog::DocumentScannerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("文档扫描与校正");
    setMinimumSize(1200, 800);

    setupUI();
    setupConnections();
}

DocumentScannerDialog::~DocumentScannerDialog()
{
}

void DocumentScannerDialog::setupUI()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #F5E6D3;
        }
        QGroupBox {
            color: #8B4513;
            border: 1px solid #D2B48C;
            margin-top: 10px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QLabel {
            color: #8B4513;
        }
        QPushButton {
            background-color: #FF8C42;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #FF9F5A;
        }
        QPushButton:pressed {
            background-color: #E6793A;
        }
        QPushButton:disabled {
            background-color: #D2B48C;
            color: #8B4513;
        }
        QSlider::groove:horizontal {
            height: 6px;
            background: #D2B48C;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FF8C42;
            width: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #D2B48C;
            border-radius: 4px;
            padding: 5px;
            color: #8B4513;
        }
    )");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // 左侧：图像显示区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(false);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setStyleSheet("background-color: #2C2C2C; border: 1px solid #444444;");

    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet("background-color: #2C2C2C;");
    m_imageLabel->setMinimumSize(600, 600);
    m_imageLabel->setText("请点击\"打开图像\"加载文档照片");

    m_scrollArea->setWidget(m_imageLabel);
    mainLayout->addWidget(m_scrollArea, 1);

    // 右侧：控制面板
    QVBoxLayout *controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(15);

    // 文件操作组
    QGroupBox *fileGroup = new QGroupBox("文件操作", this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);

    m_openBtn = new QPushButton("打开图像", this);
    m_saveBtn = new QPushButton("保存结果", this);
    m_saveBtn->setEnabled(false);

    fileLayout->addWidget(m_openBtn);
    fileLayout->addWidget(m_saveBtn);
    controlLayout->addWidget(fileGroup);

    // 文档检测组
    QGroupBox *detectGroup = new QGroupBox("文档检测", this);
    QVBoxLayout *detectLayout = new QVBoxLayout(detectGroup);

    m_detectBtn = new QPushButton("自动检测边缘", this);
    m_detectBtn->setEnabled(false);

    QLabel *thresholdLabel = new QLabel("检测灵敏度:", this);
    m_thresholdSlider = new QSlider(Qt::Horizontal, this);
    m_thresholdSlider->setRange(10, 150);
    m_thresholdSlider->setValue(50);

    detectLayout->addWidget(m_detectBtn);
    detectLayout->addWidget(thresholdLabel);
    detectLayout->addWidget(m_thresholdSlider);
    controlLayout->addWidget(detectGroup);

    // 透视校正组
    QGroupBox *transformGroup = new QGroupBox("透视校正", this);
    QVBoxLayout *transformLayout = new QVBoxLayout(transformGroup);

    m_transformBtn = new QPushButton("应用校正", this);
    m_transformBtn->setEnabled(false);

    QLabel *sizeLabel = new QLabel("输出尺寸:", this);
    m_outputSizeCombo = new QComboBox(this);
    m_outputSizeCombo->addItem("A4 (595x842)", QSize(595, 842));
    m_outputSizeCombo->addItem("Letter (612x792)", QSize(612, 792));
    m_outputSizeCombo->addItem("原始尺寸", QSize(0, 0));

    transformLayout->addWidget(m_transformBtn);
    transformLayout->addWidget(sizeLabel);
    transformLayout->addWidget(m_outputSizeCombo);
    controlLayout->addWidget(transformGroup);

    // 重置按钮
    m_resetBtn = new QPushButton("重置", this);
    m_resetBtn->setEnabled(false);
    controlLayout->addWidget(m_resetBtn);

    // 添加弹性空间
    controlLayout->addStretch();

    // 信息标签
    QLabel *infoLabel = new QLabel(
        "使用说明:\n"
        "1. 打开拍摄的文档照片\n"
        "2. 点击\"自动检测边缘\"\n"
        "3. 调整灵敏度重新检测\n"
        "4. 点击\"应用校正\"生成正视图\n"
        "5. 保存处理结果", this);
    infoLabel->setStyleSheet("color: #666666; font-size: 11px;");
    infoLabel->setWordWrap(true);
    controlLayout->addWidget(infoLabel);

    mainLayout->addLayout(controlLayout, 0);
}

void DocumentScannerDialog::setupConnections()
{
    connect(m_openBtn, &QPushButton::clicked, this, &DocumentScannerDialog::onOpenImage);
    connect(m_saveBtn, &QPushButton::clicked, this, &DocumentScannerDialog::onSaveImage);
    connect(m_detectBtn, &QPushButton::clicked, this, &DocumentScannerDialog::onAutoDetect);
    connect(m_transformBtn, &QPushButton::clicked, this, &DocumentScannerDialog::onApplyTransform);
    connect(m_resetBtn, &QPushButton::clicked, this, &DocumentScannerDialog::onReset);
    connect(m_thresholdSlider, &QSlider::valueChanged, this, &DocumentScannerDialog::onThresholdChanged);
}

void DocumentScannerDialog::onOpenImage()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "打开文档照片",
        QString(),
        "图像文件 (*.png *.jpg *.jpeg *.bmp);;所有文件 (*.*)"
    );

    if (!filePath.isEmpty()) {
        m_originalMat = cv::imread(filePath.toStdString());
        if (m_originalMat.empty()) {
            QMessageBox::warning(this, "错误", "无法加载图像文件");
            return;
        }

        // 转换为 RGB
        cv::cvtColor(m_originalMat, m_originalMat, cv::COLOR_BGR2RGB);
        m_displayMat = m_originalMat.clone();
        m_transformedMat = cv::Mat();
        m_cornersDetected = false;
        m_documentCorners.clear();

        updateDisplay();

        m_detectBtn->setEnabled(true);
        m_transformBtn->setEnabled(false);
        m_saveBtn->setEnabled(false);
        m_resetBtn->setEnabled(true);
    }
}

void DocumentScannerDialog::onSaveImage()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "保存校正后的文档",
        QString(),
        "PNG 图像 (*.png);;JPEG 图像 (*.jpg);;BMP 图像 (*.bmp)"
    );

    if (!filePath.isEmpty()) {
        cv::Mat saveMat;
        if (!m_transformedMat.empty()) {
            saveMat = m_transformedMat.clone();
        } else {
            saveMat = m_displayMat.clone();
        }

        // 转换回 BGR 保存
        cv::cvtColor(saveMat, saveMat, cv::COLOR_RGB2BGR);

        if (cv::imwrite(filePath.toStdString(), saveMat)) {
            QMessageBox::information(this, "成功", "文档已保存");
        } else {
            QMessageBox::warning(this, "错误", "保存失败");
        }
    }
}

void DocumentScannerDialog::onAutoDetect()
{
    if (m_originalMat.empty()) return;

    detectDocumentEdges();

    if (m_cornersDetected) {
        m_transformBtn->setEnabled(true);
        QMessageBox::information(this, "检测完成", "已检测到文档边缘，请查看绿色框线。\n如不准确可调整灵敏度重新检测。");
    } else {
        QMessageBox::warning(this, "未检测到", "未能检测到文档边缘，请尝试调整灵敏度或更换图片。");
    }
}

void DocumentScannerDialog::onApplyTransform()
{
    if (!m_cornersDetected || m_documentCorners.size() != 4) return;

    m_transformedMat = applyPerspectiveTransform(m_originalMat, m_documentCorners);
    m_displayMat = m_transformedMat.clone();

    updateDisplay();

    m_saveBtn->setEnabled(true);
    QMessageBox::information(this, "校正完成", "文档已校正为正视图，可以保存了。");
}

void DocumentScannerDialog::onReset()
{
    if (m_originalMat.empty()) return;

    m_displayMat = m_originalMat.clone();
    m_transformedMat = cv::Mat();
    m_cornersDetected = false;
    m_documentCorners.clear();

    updateDisplay();

    m_transformBtn->setEnabled(false);
    m_saveBtn->setEnabled(false);
}

void DocumentScannerDialog::onThresholdChanged(int value)
{
    m_cannyThreshold = value;
    // 如果已经加载了图像，自动重新检测
    if (!m_originalMat.empty() && m_transformedMat.empty()) {
        detectDocumentEdges();
        updateDisplay();
    }
}

void DocumentScannerDialog::detectDocumentEdges()
{
    if (m_originalMat.empty()) return;

    // 创建副本用于显示检测线
    m_displayMat = m_originalMat.clone();

    // 图像尺寸
    int imgWidth = m_originalMat.cols;
    int imgHeight = m_originalMat.rows;
    int imgArea = imgWidth * imgHeight;

    // 预处理 - 使用自适应阈值来更好地区分纸张和背景
    cv::Mat gray, blurred, edges;
    cv::cvtColor(m_originalMat, gray, cv::COLOR_RGB2GRAY);

    // 使用双边滤波保留边缘同时去噪
    cv::bilateralFilter(gray, blurred, 9, 75, 75);

    // Canny 边缘检测 - 使用更低的阈值来检测更多边缘
    int lowThreshold = std::max(10, m_cannyThreshold - 20);
    int highThreshold = m_cannyThreshold + 20;
    cv::Canny(blurred, edges, lowThreshold, highThreshold);

    // 膨胀连接断裂的边缘
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::dilate(edges, edges, kernel);

    // 调试：显示边缘检测结果
    qDebug() << "图像尺寸:" << imgWidth << "x" << imgHeight << "总面积:" << imgArea;
    qDebug() << "Canny阈值:" << lowThreshold << "-" << highThreshold;

    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    qDebug() << "找到轮廓数量:" << contours.size();

    // 寻找最佳的四边形（优先选择接近纸张的轮廓）
    std::vector<cv::Point> bestContour;
    double bestScore = 0;

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        qDebug() << "轮廓" << i << "面积:" << area;

        // 过滤太小或太大的区域
        // 面积应该占图像的 20% - 90%
        if (area < imgArea * 0.2 || area > imgArea * 0.95) {
            qDebug() << "  -> 面积不符合要求 (" << (area / imgArea * 100) << "%)";
            continue;
        }

        // 获取最小外接矩形
        cv::RotatedRect minRect = cv::minAreaRect(contours[i]);
        double rectArea = minRect.size.width * minRect.size.height;
        double extent = area / rectArea; // 轮廓面积 / 矩形面积

        qDebug() << "  外接矩形面积:" << rectArea << "填充率:" << extent;

        // 轮廓近似
        std::vector<cv::Point> approx;
        double epsilon = cv::arcLength(contours[i], true) * 0.02;
        cv::approxPolyDP(contours[i], approx, epsilon, true);

        qDebug() << "  近似顶点数:" << approx.size() << "epsilon:" << epsilon;

        // 计算轮廓中心
        cv::Moments m = cv::moments(contours[i]);
        if (m.m00 == 0) continue;
        cv::Point center(m.m10 / m.m00, m.m01 / m.m00);

        // 检查轮廓是否太靠近图像边缘（排除图像边框）
        int margin = 20; // 边缘余量
        bool tooCloseToEdge = false;
        for (const auto& pt : approx) {
            if (pt.x < margin || pt.x > imgWidth - margin ||
                pt.y < margin || pt.y > imgHeight - margin) {
                // 检查这个点是否是图像角点
                bool isImageCorner = (pt.x < margin && pt.y < margin) ||
                                    (pt.x > imgWidth - margin && pt.y < margin) ||
                                    (pt.x < margin && pt.y > imgHeight - margin) ||
                                    (pt.x > imgWidth - margin && pt.y > imgHeight - margin);
                if (isImageCorner) {
                    tooCloseToEdge = true;
                    break;
                }
            }
        }

        if (tooCloseToEdge) {
            qDebug() << "  -> 太靠近图像边缘，可能是图像边框";
            continue;
        }

        // 评分系统：综合考虑面积、填充率、顶点数
        double score = area * extent;

        // 优先选择4-6个顶点的轮廓
        if (approx.size() >= 4 && approx.size() <= 6) {
            score *= 1.5;
        }

        // 优先选择接近中心的轮廓
        double distToCenter = std::sqrt(std::pow(center.x - imgWidth/2.0, 2) +
                                       std::pow(center.y - imgHeight/2.0, 2));
        double centerScore = 1.0 - (distToCenter / (std::sqrt(imgWidth*imgWidth + imgHeight*imgHeight) / 2.0));
        score *= (0.5 + 0.5 * centerScore);

        qDebug() << "  评分:" << score << "(当前最佳:" << bestScore << ")";

        if (score > bestScore) {
            bestScore = score;
            // 使用最小外接矩形的4个角点
            cv::Point2f vertices[4];
            minRect.points(vertices);
            bestContour.clear();
            for (int j = 0; j < 4; j++) {
                bestContour.push_back(cv::Point(static_cast<int>(vertices[j].x),
                                               static_cast<int>(vertices[j].y)));
            }
            qDebug() << "  -> 新的最佳轮廓";
        }
    }

    // 备用方案：如果上面的方法没找到，使用传统的最大轮廓方法
    if (bestContour.empty() && !contours.empty()) {
        qDebug() << "尝试备用方案：寻找最大轮廓的最小外接矩形";
        double maxContourArea = 0;
        std::vector<cv::Point> largestContour;

        for (const auto &contour : contours) {
            double area = cv::contourArea(contour);
            // 排除图像边框（面积接近图像总面积的）
            if (area > imgArea * 0.95) continue;
            if (area > maxContourArea) {
                maxContourArea = area;
                largestContour = contour;
            }
        }

        if (!largestContour.empty() && maxContourArea > imgArea * 0.1) {
            cv::RotatedRect minRect = cv::minAreaRect(largestContour);
            cv::Point2f vertices[4];
            minRect.points(vertices);
            bestContour.clear();
            for (int i = 0; i < 4; i++) {
                bestContour.push_back(cv::Point(static_cast<int>(vertices[i].x),
                                               static_cast<int>(vertices[i].y)));
            }
            qDebug() << "使用最小外接矩形，面积:" << maxContourArea;
        }
    }

    if (!bestContour.empty()) {
        m_documentCorners = orderPoints(bestContour);
        m_cornersDetected = true;

        qDebug() << "检测到角点:";
        for (size_t i = 0; i < m_documentCorners.size(); i++) {
            qDebug() << "  点" << i << ":(" << m_documentCorners[i].x << "," << m_documentCorners[i].y << ")";
        }

        // 在原始图像副本上绘制检测框线
        m_displayMat = m_originalMat.clone();
        for (size_t i = 0; i < m_documentCorners.size(); i++) {
            // 绘制角点（红色圆圈）
            cv::circle(m_displayMat, m_documentCorners[i], 12, cv::Scalar(255, 0, 0), -1);
            // 绘制边框线（绿色粗线）
            cv::line(m_displayMat, m_documentCorners[i],
                     m_documentCorners[(i + 1) % m_documentCorners.size()],
                     cv::Scalar(0, 255, 0), 4);
        }

        // 刷新显示
        updateDisplay();
    } else {
        m_cornersDetected = false;
        m_documentCorners.clear();
        qDebug() << "未找到合适的文档轮廓";
    }
}

std::vector<cv::Point> DocumentScannerDialog::orderPoints(const std::vector<cv::Point> &pts)
{
    // 如果输入超过4个点，先找到最接近矩形的4个点
    std::vector<cv::Point> inputPts = pts;
    if (pts.size() > 4) {
        // 使用最小外接矩形来获取4个角点
        cv::RotatedRect minRect = cv::minAreaRect(pts);
        cv::Point2f vertices[4];
        minRect.points(vertices);
        inputPts.clear();
        for (int i = 0; i < 4; i++) {
            inputPts.push_back(cv::Point(static_cast<int>(vertices[i].x), static_cast<int>(vertices[i].y)));
        }
    }

    // 确保有4个点
    if (inputPts.size() < 4) {
        // 如果不足4个点，用原始点填充
        std::vector<cv::Point> result = inputPts;
        while (result.size() < 4) {
            result.push_back(inputPts.empty() ? cv::Point(0, 0) : inputPts[0]);
        }
        return result;
    }

    std::vector<cv::Point> ordered(4);

    // 计算每个点的坐标和 (x + y) 和差 (y - x)
    // 左上：和最小，右下：和最大
    // 右上：y-x 最大，左下：y-x 最小

    int tl_idx = 0, br_idx = 0, tr_idx = 0, bl_idx = 0;
    int minSum = INT_MAX, maxSum = INT_MIN;
    int minDiff = INT_MAX, maxDiff = INT_MIN;

    for (size_t i = 0; i < 4; i++) {
        int sum = inputPts[i].x + inputPts[i].y;
        int diff = inputPts[i].y - inputPts[i].x;

        if (sum < minSum) {
            minSum = sum;
            tl_idx = i;
        }
        if (sum > maxSum) {
            maxSum = sum;
            br_idx = i;
        }
        if (diff < minDiff) {
            minDiff = diff;
            bl_idx = i;
        }
        if (diff > maxDiff) {
            maxDiff = diff;
            tr_idx = i;
        }
    }

    ordered[0] = inputPts[tl_idx]; // 左上
    ordered[1] = inputPts[tr_idx]; // 右上
    ordered[2] = inputPts[br_idx]; // 右下
    ordered[3] = inputPts[bl_idx]; // 左下

    qDebug() << "角点排序: TL(" << ordered[0].x << "," << ordered[0].y << ")"
             << " TR(" << ordered[1].x << "," << ordered[1].y << ")"
             << " BR(" << ordered[2].x << "," << ordered[2].y << ")"
             << " BL(" << ordered[3].x << "," << ordered[3].y << ")";

    return ordered;
}

cv::Mat DocumentScannerDialog::applyPerspectiveTransform(const cv::Mat &image, const std::vector<cv::Point> &corners)
{
    // 获取输出尺寸
    QSize outputSize = m_outputSizeCombo->currentData().toSize();
    int maxWidth, maxHeight;

    if (outputSize.width() > 0) {
        maxWidth = outputSize.width();
        maxHeight = outputSize.height();
    } else {
        // 计算原始文档的宽度和高度
        int widthA = std::sqrt(std::pow(corners[1].x - corners[0].x, 2) + std::pow(corners[1].y - corners[0].y, 2));
        int widthB = std::sqrt(std::pow(corners[2].x - corners[3].x, 2) + std::pow(corners[2].y - corners[3].y, 2));
        maxWidth = std::max(widthA, widthB);

        int heightA = std::sqrt(std::pow(corners[3].x - corners[0].x, 2) + std::pow(corners[3].y - corners[0].y, 2));
        int heightB = std::sqrt(std::pow(corners[2].x - corners[1].x, 2) + std::pow(corners[2].y - corners[1].y, 2));
        maxHeight = std::max(heightA, heightB);
    }

    // 定义目标点
    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, 0),
        cv::Point2f(maxWidth - 1, 0),
        cv::Point2f(maxWidth - 1, maxHeight - 1),
        cv::Point2f(0, maxHeight - 1)
    };

    std::vector<cv::Point2f> srcPoints;
    for (const auto &pt : corners) {
        srcPoints.push_back(cv::Point2f(pt.x, pt.y));
    }

    // 计算透视变换矩阵
    cv::Mat transformMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);

    // 应用变换
    cv::Mat result;
    cv::warpPerspective(image, result, transformMatrix, cv::Size(maxWidth, maxHeight));

    return result;
}

void DocumentScannerDialog::updateDisplay()
{
    if (m_displayMat.empty()) return;

    m_displayImage = CvMatToQImage(m_displayMat);
    m_imageLabel->setPixmap(QPixmap::fromImage(m_displayImage).scaled(
        m_displayImage.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_imageLabel->resize(m_displayImage.size());
}

cv::Mat DocumentScannerDialog::QImageToCvMat(const QImage &image)
{
    switch (image.format()) {
    case QImage::Format_RGB888:
        return cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    case QImage::Format_Grayscale8:
        return cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    default:
        return cv::Mat();
    }
}

QImage DocumentScannerDialog::CvMatToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC3:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).copy();
    case CV_8UC4:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32).copy();
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    default:
        return QImage();
    }
}
