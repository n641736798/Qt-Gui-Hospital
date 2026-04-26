#include "imageviewerdialog.h"
#include "medicalimageviewer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

ImageViewerDialog::ImageViewerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("医学影像查看器");
    setMinimumSize(1000, 700);
    
    setupUI();
    setupConnections();
}

ImageViewerDialog::~ImageViewerDialog()
{
}

void ImageViewerDialog::setupUI()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #F5E6D3;
        }
        QLabel {
            color: #8B4513;
            font-weight: bold;
        }
        QLineEdit {
            background-color: #FFFFFF;
            border: 1px solid #D2B48C;
            border-radius: 4px;
            padding: 5px;
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
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // 顶部工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_openBtn = new QPushButton("打开图像", this);
    m_saveBtn = new QPushButton("保存图像", this);
    m_saveBtn->setEnabled(false);
    
    m_infoLabel = new QLabel("未加载图像", this);
    m_infoLabel->setStyleSheet("color: #666666;");
    
    toolbarLayout->addWidget(m_openBtn);
    toolbarLayout->addWidget(m_saveBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_infoLabel);
    
    mainLayout->addLayout(toolbarLayout);

    // 图像查看器
    m_imageViewer = new MedicalImageViewer(this);
    mainLayout->addWidget(m_imageViewer, 1);
}

void ImageViewerDialog::setupConnections()
{
    connect(m_openBtn, &QPushButton::clicked, this, &ImageViewerDialog::onOpenImage);
    connect(m_saveBtn, &QPushButton::clicked, this, &ImageViewerDialog::onSaveImage);
    connect(m_imageViewer, &MedicalImageViewer::imageLoaded, this, &ImageViewerDialog::onImageLoaded);
}

bool ImageViewerDialog::loadImage(const QString &filePath)
{
    return m_imageViewer->loadImage(filePath);
}

void ImageViewerDialog::onOpenImage()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "打开医学影像",
        QString(),
        "图像文件 (*.png *.jpg *.jpeg *.bmp *.tif *.tiff *.dcm);;所有文件 (*.*)"
    );
    
    if (!filePath.isEmpty()) {
        if (m_imageViewer->loadImage(filePath)) {
            m_currentFilePath = filePath;
            m_saveBtn->setEnabled(true);
        } else {
            QMessageBox::warning(this, "错误", "无法加载图像文件:\n" + filePath);
        }
    }
}

void ImageViewerDialog::onSaveImage()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "保存图像",
        QString(),
        "PNG 图像 (*.png);;JPEG 图像 (*.jpg);;BMP 图像 (*.bmp)"
    );
    
    if (!filePath.isEmpty()) {
        QImage image = m_imageViewer->currentImage();
        if (image.save(filePath)) {
            QMessageBox::information(this, "成功", "图像已保存到:\n" + filePath);
        } else {
            QMessageBox::warning(this, "错误", "保存图像失败");
        }
    }
}

void ImageViewerDialog::onImageLoaded(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString info = QString("图像: %1 | 大小: %2 KB")
        .arg(fileInfo.fileName())
        .arg(fileInfo.size() / 1024);
    m_infoLabel->setText(info);
}
