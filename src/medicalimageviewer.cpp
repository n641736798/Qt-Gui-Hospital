#include "medicalimageviewer.h"

#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QDebug>

MedicalImageViewer::MedicalImageViewer(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

MedicalImageViewer::~MedicalImageViewer()
{
}

void MedicalImageViewer::setupUI()
{
    setStyleSheet(R"(
        MedicalImageViewer {
            background-color: #2C2C2C;
        }
        QGroupBox {
            color: #FFFFFF;
            border: 1px solid #555555;
            margin-top: 10px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QLabel {
            color: #FFFFFF;
        }
        QPushButton {
            background-color: #4A4A4A;
            color: #FFFFFF;
            border: 1px solid #666666;
            padding: 8px 15px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #5A5A5A;
        }
        QPushButton:pressed {
            background-color: #3A3A3A;
        }
        QPushButton:checked {
            background-color: #FF8C42;
        }
        QSlider::groove:horizontal {
            height: 6px;
            background: #4A4A4A;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FF8C42;
            width: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
    )");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 左侧：图像显示区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet("background-color: #1E1E1E; border: 1px solid #444444;");
    
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet("background-color: #1E1E1E;");
    m_imageLabel->setMinimumSize(400, 400);
    m_imageLabel->setText("暂无图像\n请点击打开图像按钮加载医学影像");
    
    m_scrollArea->setWidget(m_imageLabel);
    mainLayout->addWidget(m_scrollArea, 1);

    // 右侧：控制面板
    QVBoxLayout *controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(15);

    // 图像调整组
    QGroupBox *adjustGroup = new QGroupBox("图像调整", this);
    QGridLayout *adjustLayout = new QGridLayout(adjustGroup);
    
    QLabel *brightnessLabel = new QLabel("亮度:", this);
    m_brightnessSlider = new QSlider(Qt::Horizontal, this);
    m_brightnessSlider->setRange(-100, 100);
    m_brightnessSlider->setValue(0);
    
    QLabel *contrastLabel = new QLabel("对比度:", this);
    m_contrastSlider = new QSlider(Qt::Horizontal, this);
    m_contrastSlider->setRange(0, 200);
    m_contrastSlider->setValue(100);
    
    adjustLayout->addWidget(brightnessLabel, 0, 0);
    adjustLayout->addWidget(m_brightnessSlider, 0, 1);
    adjustLayout->addWidget(contrastLabel, 1, 0);
    adjustLayout->addWidget(m_contrastSlider, 1, 1);
    
    controlLayout->addWidget(adjustGroup);

    // 缩放控制组
    QGroupBox *zoomGroup = new QGroupBox("缩放控制", this);
    QHBoxLayout *zoomLayout = new QHBoxLayout(zoomGroup);
    
    m_zoomOutBtn = new QPushButton("缩小", this);
    m_zoomInBtn = new QPushButton("放大", this);
    m_resetBtn = new QPushButton("重置", this);
    
    zoomLayout->addWidget(m_zoomOutBtn);
    zoomLayout->addWidget(m_zoomInBtn);
    zoomLayout->addWidget(m_resetBtn);
    
    controlLayout->addWidget(zoomGroup);

    // 旋转控制组
    QGroupBox *rotateGroup = new QGroupBox("旋转/翻转", this);
    QHBoxLayout *rotateLayout = new QHBoxLayout(rotateGroup);
    
    m_rotateLeftBtn = new QPushButton("左转", this);
    m_rotateRightBtn = new QPushButton("右转", this);
    
    rotateLayout->addWidget(m_rotateLeftBtn);
    rotateLayout->addWidget(m_rotateRightBtn);
    
    controlLayout->addWidget(rotateGroup);

    // 标注工具组
    QGroupBox *annotationGroup = new QGroupBox("标注工具", this);
    QVBoxLayout *annotationLayout = new QVBoxLayout(annotationGroup);
    
    m_annotationBtn = new QPushButton("启用矩形标注", this);
    m_annotationBtn->setCheckable(true);
    
    QPushButton *clearAnnotationBtn = new QPushButton("清除标注", this);
    
    annotationLayout->addWidget(m_annotationBtn);
    annotationLayout->addWidget(clearAnnotationBtn);
    
    controlLayout->addWidget(annotationGroup);

    // 添加弹性空间
    controlLayout->addStretch();

    // 信息标签
    QLabel *infoLabel = new QLabel(
        "操作说明:\n"
        "• 滚轮缩放图像\n"
        "• 拖拽移动图像\n"
        "• 标注模式下拖拽绘制矩形", this);
    infoLabel->setStyleSheet("color: #AAAAAA; font-size: 11px;");
    infoLabel->setWordWrap(true);
    controlLayout->addWidget(infoLabel);

    mainLayout->addLayout(controlLayout, 0);

    // 连接标注清除按钮
    connect(clearAnnotationBtn, &QPushButton::clicked, this, &MedicalImageViewer::clearAnnotations);
}

void MedicalImageViewer::setupConnections()
{
    // 亮度和对比度
    connect(m_brightnessSlider, &QSlider::valueChanged, this, &MedicalImageViewer::setBrightness);
    connect(m_contrastSlider, &QSlider::valueChanged, this, &MedicalImageViewer::setContrast);
    
    // 缩放按钮
    connect(m_zoomInBtn, &QPushButton::clicked, [this]() {
        setZoom(m_zoomFactor * 1.25);
    });
    connect(m_zoomOutBtn, &QPushButton::clicked, [this]() {
        setZoom(m_zoomFactor * 0.8);
    });
    connect(m_resetBtn, &QPushButton::clicked, this, &MedicalImageViewer::resetAdjustments);
    
    // 旋转按钮
    connect(m_rotateLeftBtn, &QPushButton::clicked, this, &MedicalImageViewer::rotateLeft);
    connect(m_rotateRightBtn, &QPushButton::clicked, this, &MedicalImageViewer::rotateRight);
    
    // 标注按钮
    connect(m_annotationBtn, &QPushButton::toggled, this, &MedicalImageViewer::enableAnnotation);
}

bool MedicalImageViewer::loadImage(const QString &filePath)
{
    m_originalMat = cv::imread(filePath.toStdString(), cv::IMREAD_UNCHANGED);
    if (m_originalMat.empty()) {
        qDebug() << "Failed to load image:" << filePath;
        return false;
    }
    
    // 转换为 RGB 格式
    if (m_originalMat.channels() == 3) {
        cv::cvtColor(m_originalMat, m_originalMat, cv::COLOR_BGR2RGB);
    } else if (m_originalMat.channels() == 4) {
        cv::cvtColor(m_originalMat, m_originalMat, cv::COLOR_BGRA2RGBA);
    }
    
    resetAdjustments();
    emit imageLoaded(filePath);
    return true;
}

bool MedicalImageViewer::loadImage(const QImage &image)
{
    m_originalMat = QImageToCvMat(image);
    if (m_originalMat.empty()) {
        return false;
    }
    
    resetAdjustments();
    return true;
}

QImage MedicalImageViewer::currentImage() const
{
    return m_displayImage;
}

void MedicalImageViewer::clear()
{
    m_originalMat.release();
    m_processedMat.release();
    m_displayImage = QImage();
    m_imageLabel->setText("暂无图像\n请点击打开图像按钮加载医学影像");
    clearAnnotations();
}

void MedicalImageViewer::setBrightness(int value)
{
    m_brightness = value;
    applyAdjustments();
}

void MedicalImageViewer::setContrast(int value)
{
    m_contrast = value;
    applyAdjustments();
}

void MedicalImageViewer::setZoom(double factor)
{
    if (factor < 0.1) factor = 0.1;
    if (factor > 10.0) factor = 10.0;
    
    m_zoomFactor = factor;
    updateDisplay();
    emit zoomChanged(m_zoomFactor);
}

void MedicalImageViewer::resetAdjustments()
{
    m_brightness = 0;
    m_contrast = 100;
    m_zoomFactor = 1.0;
    m_rotation = 0;
    m_flipH = false;
    m_flipV = false;
    
    m_brightnessSlider->setValue(0);
    m_contrastSlider->setValue(100);
    
    if (!m_originalMat.empty()) {
        applyAdjustments();
    }
}

void MedicalImageViewer::rotateLeft()
{
    m_rotation = (m_rotation - 90) % 360;
    applyAdjustments();
}

void MedicalImageViewer::rotateRight()
{
    m_rotation = (m_rotation + 90) % 360;
    applyAdjustments();
}

void MedicalImageViewer::flipHorizontal()
{
    m_flipH = !m_flipH;
    applyAdjustments();
}

void MedicalImageViewer::flipVertical()
{
    m_flipV = !m_flipV;
    applyAdjustments();
}

void MedicalImageViewer::enableAnnotation(bool enable)
{
    m_annotationEnabled = enable;
    if (enable) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MedicalImageViewer::clearAnnotations()
{
    m_annotations.clear();
    updateDisplay();
}

void MedicalImageViewer::applyAdjustments()
{
    if (m_originalMat.empty()) return;
    
    // 复制原始图像
    m_processedMat = m_originalMat.clone();
    
    // 应用亮度和对比度调整
    double alpha = m_contrast / 100.0;
    int beta = m_brightness;
    
    m_processedMat.convertTo(m_processedMat, -1, alpha, beta);
    
    // 裁剪到有效范围
    cv::Mat temp;
    m_processedMat.convertTo(temp, CV_8U);
    m_processedMat = temp;
    
    // 应用旋转
    if (m_rotation != 0) {
        cv::Point2f center(m_processedMat.cols / 2.0f, m_processedMat.rows / 2.0f);
        cv::Mat rotMatrix = cv::getRotationMatrix2D(center, m_rotation, 1.0);
        cv::warpAffine(m_processedMat, m_processedMat, rotMatrix, m_processedMat.size());
    }
    
    // 应用翻转
    if (m_flipH) {
        cv::flip(m_processedMat, m_processedMat, 1);
    }
    if (m_flipV) {
        cv::flip(m_processedMat, m_processedMat, 0);
    }
    
    updateDisplay();
}

void MedicalImageViewer::updateDisplay()
{
    if (m_processedMat.empty()) return;
    
    // 转换为 QImage
    m_displayImage = CvMatToQImage(m_processedMat);
    
    // 应用缩放
    if (m_zoomFactor != 1.0) {
        int newWidth = static_cast<int>(m_displayImage.width() * m_zoomFactor);
        int newHeight = static_cast<int>(m_displayImage.height() * m_zoomFactor);
        m_displayImage = m_displayImage.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    // 绘制标注
    if (!m_annotations.isEmpty()) {
        QPainter painter(&m_displayImage);
        painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));
        for (const QRect &rect : m_annotations) {
            QRect scaledRect(
                static_cast<int>(rect.x() * m_zoomFactor),
                static_cast<int>(rect.y() * m_zoomFactor),
                static_cast<int>(rect.width() * m_zoomFactor),
                static_cast<int>(rect.height() * m_zoomFactor)
            );
            painter.drawRect(scaledRect);
        }
    }
    
    m_imageLabel->setPixmap(QPixmap::fromImage(m_displayImage));
    m_imageLabel->resize(m_displayImage.size());
}

cv::Mat MedicalImageViewer::QImageToCvMat(const QImage &image)
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
        qDebug() << "Unsupported QImage format";
        return cv::Mat();
    }
}

QImage MedicalImageViewer::CvMatToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC3:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    case CV_8UC4:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    default:
        qDebug() << "Unsupported cv::Mat type:" << mat.type();
        return QImage();
    }
}

void MedicalImageViewer::mousePressEvent(QMouseEvent *event)
{
    if (m_annotationEnabled && event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_annotationStart = event->pos();
    }
}

void MedicalImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDrawing && m_annotationEnabled) {
        // 实时显示绘制效果（可选）
    }
}

void MedicalImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isDrawing && m_annotationEnabled) {
        m_isDrawing = false;
        
        QPoint end = event->pos();
        QRect rect(m_annotationStart, end);
        rect = rect.normalized();
        
        // 转换到原始图像坐标
        QRect originalRect(
            static_cast<int>(rect.x() / m_zoomFactor),
            static_cast<int>(rect.y() / m_zoomFactor),
            static_cast<int>(rect.width() / m_zoomFactor),
            static_cast<int>(rect.height() / m_zoomFactor)
        );
        
        m_annotations.append(originalRect);
        updateDisplay();
    }
}

void MedicalImageViewer::wheelEvent(QWheelEvent *event)
{
    if (m_processedMat.empty()) return;
    
    double delta = event->angleDelta().y() / 120.0;
    double newFactor = m_zoomFactor * (delta > 0 ? 1.1 : 0.9);
    setZoom(newFactor);
}

void MedicalImageViewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}
