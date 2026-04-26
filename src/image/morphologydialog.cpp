#include "morphologydialog.h"

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
#include <QDebug>

MorphologyDialog::MorphologyDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("形态学操作");
    setMinimumSize(1200, 800);

    setupUI();
    setupConnections();
}

MorphologyDialog::~MorphologyDialog()
{
}

void MorphologyDialog::setupUI()
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
    m_imageLabel->setText("请点击\"打开图像\"加载图像");

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

    // 参数设置组
    QGroupBox *paramGroup = new QGroupBox("参数设置", this);
    QVBoxLayout *paramLayout = new QVBoxLayout(paramGroup);

    m_kernelSizeLabel = new QLabel("核大小: 5", this);
    m_kernelSizeSlider = new QSlider(Qt::Horizontal, this);
    m_kernelSizeSlider->setRange(3, 21);
    m_kernelSizeSlider->setValue(5);
    m_kernelSizeSlider->setSingleStep(2);

    m_iterationsLabel = new QLabel("迭代次数: 1", this);
    m_iterationsSlider = new QSlider(Qt::Horizontal, this);
    m_iterationsSlider->setRange(1, 10);
    m_iterationsSlider->setValue(1);

    paramLayout->addWidget(m_kernelSizeLabel);
    paramLayout->addWidget(m_kernelSizeSlider);
    paramLayout->addSpacing(10);
    paramLayout->addWidget(m_iterationsLabel);
    paramLayout->addWidget(m_iterationsSlider);
    controlLayout->addWidget(paramGroup);

    // 形态学操作组
    QGroupBox *morphGroup = new QGroupBox("形态学操作", this);
    QVBoxLayout *morphLayout = new QVBoxLayout(morphGroup);

    m_erosionBtn = new QPushButton("腐蚀 (Erosion)", this);
    m_dilationBtn = new QPushButton("膨胀 (Dilation)", this);
    m_openingBtn = new QPushButton("开运算 (Opening)", this);
    m_closingBtn = new QPushButton("闭运算 (Closing)", this);
    m_gradientBtn = new QPushButton("形态学梯度 (Gradient)", this);

    m_erosionBtn->setEnabled(false);
    m_dilationBtn->setEnabled(false);
    m_openingBtn->setEnabled(false);
    m_closingBtn->setEnabled(false);
    m_gradientBtn->setEnabled(false);

    morphLayout->addWidget(m_erosionBtn);
    morphLayout->addWidget(m_dilationBtn);
    morphLayout->addWidget(m_openingBtn);
    morphLayout->addWidget(m_closingBtn);
    morphLayout->addWidget(m_gradientBtn);
    controlLayout->addWidget(morphGroup);

    // 重置按钮
    m_resetBtn = new QPushButton("重置", this);
    m_resetBtn->setEnabled(false);
    controlLayout->addWidget(m_resetBtn);

    // 添加弹性空间
    controlLayout->addStretch();

    // 信息标签
    QLabel *infoLabel = new QLabel(
        "使用说明:\n"
        "• 腐蚀：缩小亮区域，去除小噪点\n"
        "• 膨胀：扩大亮区域，连接断裂\n"
        "• 开运算：先腐蚀后膨胀，平滑轮廓\n"
        "• 闭运算：先膨胀后腐蚀，填充孔洞\n"
        "• 梯度：膨胀减腐蚀，提取边缘", this);
    infoLabel->setStyleSheet("color: #666666; font-size: 11px;");
    infoLabel->setWordWrap(true);
    controlLayout->addWidget(infoLabel);

    mainLayout->addLayout(controlLayout, 0);
}

void MorphologyDialog::setupConnections()
{
    connect(m_openBtn, &QPushButton::clicked, this, &MorphologyDialog::onOpenImage);
    connect(m_saveBtn, &QPushButton::clicked, this, &MorphologyDialog::onSaveImage);
    connect(m_erosionBtn, &QPushButton::clicked, this, &MorphologyDialog::onApplyErosion);
    connect(m_dilationBtn, &QPushButton::clicked, this, &MorphologyDialog::onApplyDilation);
    connect(m_openingBtn, &QPushButton::clicked, this, &MorphologyDialog::onApplyOpening);
    connect(m_closingBtn, &QPushButton::clicked, this, &MorphologyDialog::onApplyClosing);
    connect(m_gradientBtn, &QPushButton::clicked, this, &MorphologyDialog::onApplyGradient);
    connect(m_resetBtn, &QPushButton::clicked, this, &MorphologyDialog::onReset);
    connect(m_kernelSizeSlider, &QSlider::valueChanged, this, &MorphologyDialog::onKernelSizeChanged);
    connect(m_iterationsSlider, &QSlider::valueChanged, this, &MorphologyDialog::onIterationsChanged);
}

void MorphologyDialog::onOpenImage()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "打开图像",
        QString(),
        "图像文件 (*.png *.jpg *.jpeg *.bmp);;所有文件 (*.*)"
    );

    if (!filePath.isEmpty()) {
        m_originalMat = cv::imread(filePath.toStdString());
        if (m_originalMat.empty()) {
            QMessageBox::warning(this, "错误", "无法加载图像文件");
            return;
        }

        cv::cvtColor(m_originalMat, m_originalMat, cv::COLOR_BGR2RGB);
        m_processedMat = m_originalMat.clone();

        updateDisplay();

        m_erosionBtn->setEnabled(true);
        m_dilationBtn->setEnabled(true);
        m_openingBtn->setEnabled(true);
        m_closingBtn->setEnabled(true);
        m_gradientBtn->setEnabled(true);
        m_saveBtn->setEnabled(true);
        m_resetBtn->setEnabled(true);
    }
}

void MorphologyDialog::onSaveImage()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "保存结果",
        QString(),
        "PNG 图像 (*.png);;JPEG 图像 (*.jpg);;BMP 图像 (*.bmp)"
    );

    if (!filePath.isEmpty()) {
        cv::Mat saveMat = m_processedMat.clone();
        cv::cvtColor(saveMat, saveMat, cv::COLOR_RGB2BGR);

        if (cv::imwrite(filePath.toStdString(), saveMat)) {
            QMessageBox::information(this, "成功", "图像已保存");
        } else {
            QMessageBox::warning(this, "错误", "保存失败");
        }
    }
}

void MorphologyDialog::onApplyErosion()
{
    applyMorphology(cv::MORPH_ERODE);
}

void MorphologyDialog::onApplyDilation()
{
    applyMorphology(cv::MORPH_DILATE);
}

void MorphologyDialog::onApplyOpening()
{
    applyMorphology(cv::MORPH_OPEN);
}

void MorphologyDialog::onApplyClosing()
{
    applyMorphology(cv::MORPH_CLOSE);
}

void MorphologyDialog::onApplyGradient()
{
    applyMorphology(cv::MORPH_GRADIENT);
}

void MorphologyDialog::applyMorphology(int operation)
{
    if (m_originalMat.empty()) return;

    cv::Mat gray, result;

    if (m_processedMat.channels() == 3) {
        cv::cvtColor(m_processedMat, gray, cv::COLOR_RGB2GRAY);
    } else {
        gray = m_processedMat.clone();
    }

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_kernelSize, m_kernelSize));

    if (operation == cv::MORPH_ERODE) {
        cv::erode(gray, result, kernel, cv::Point(-1, -1), m_iterations);
    } else if (operation == cv::MORPH_DILATE) {
        cv::dilate(gray, result, kernel, cv::Point(-1, -1), m_iterations);
    } else {
        cv::morphologyEx(gray, result, operation, kernel, cv::Point(-1, -1), m_iterations);
    }

    if (operation == cv::MORPH_GRADIENT) {
        cv::cvtColor(result, m_processedMat, cv::COLOR_GRAY2RGB);
    } else if (m_originalMat.channels() == 3) {
        cv::cvtColor(result, m_processedMat, cv::COLOR_GRAY2RGB);
    } else {
        m_processedMat = result;
    }

    updateDisplay();
}

void MorphologyDialog::onReset()
{
    if (m_originalMat.empty()) return;

    m_processedMat = m_originalMat.clone();
    updateDisplay();
}

void MorphologyDialog::onKernelSizeChanged(int value)
{
    // 确保奇数
    if (value % 2 == 0) {
        value = value + 1;
        m_kernelSizeSlider->setValue(value);
    }
    m_kernelSize = value;
    m_kernelSizeLabel->setText(QString("核大小: %1").arg(value));
}

void MorphologyDialog::onIterationsChanged(int value)
{
    m_iterations = value;
    m_iterationsLabel->setText(QString("迭代次数: %1").arg(value));
}

void MorphologyDialog::updateDisplay()
{
    if (m_processedMat.empty()) return;

    m_displayImage = CvMatToQImage(m_processedMat);
    m_imageLabel->setPixmap(QPixmap::fromImage(m_displayImage).scaled(
        m_displayImage.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_imageLabel->resize(m_displayImage.size());
}

cv::Mat MorphologyDialog::QImageToCvMat(const QImage &image)
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

QImage MorphologyDialog::CvMatToQImage(const cv::Mat &mat)
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
