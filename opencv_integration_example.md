# OpenCV 医学影像查看器集成指南

## 功能介绍

本项目已集成基于 OpenCV 的医学影像查看器，支持以下功能：

### 1. 图像加载与显示
- 支持格式：PNG, JPG, JPEG, BMP, TIFF, DICOM
- 大图像流畅浏览

### 2. 图像调整
- **亮度调节**：-100 到 +100
- **对比度调节**：0% 到 200%
- **缩放**：10% 到 1000%，支持鼠标滚轮
- **旋转**：左转/右转 90度
- **翻转**：水平翻转、垂直翻转

### 3. 标注工具
- 矩形框标注病灶区域
- 标注结果可随图像一起保存

## 编译配置

### Windows (MinGW/MSVC)

1. 下载并安装 OpenCV：https://opencv.org/releases/

2. 修改 `Hospital_reorganized.pro`，取消注释并修改 OpenCV 路径：

```pro
win32 {
    INCLUDEPATH += C:/opencv/build/include
    LIBS += -LC:/opencv/build/x64/vc16/lib \
            -lopencv_core490 \
            -lopencv_imgproc490 \
            -lopencv_imgcodecs490 \
            -lopencv_highgui490
}
```

### Linux

```pro
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
}
```

或者手动指定：
```pro
unix {
    INCLUDEPATH += /usr/include/opencv4
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui
}
```

### macOS

```pro
macx {
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/usr/local/lib \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_imgcodecs \
            -lopencv_highgui
}
```

## 使用示例

### 方式一：独立对话框

```cpp
#include "imageviewerdialog.h"

// 在需要打开影像查看器的地方：
void MainWindow::onViewMedicalImage()
{
    ImageViewerDialog dialog(this);
    
    // 可选：预加载图像
    // dialog.loadImage("path/to/image.png");
    
    dialog.exec();
}
```

### 方式二：嵌入到现有页面

```cpp
#include "medicalimageviewer.h"

// 在某个 Widget 的构造函数中：
void SomeWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // 创建影像查看器
    MedicalImageViewer *viewer = new MedicalImageViewer(this);
    layout->addWidget(viewer);
    
    // 加载图像
    viewer->loadImage("path/to/medical/image.png");
}
```

### 方式三：与患者系统集成

```cpp
// 在患者详情页面添加"查看影像"按钮
void PatientWidget::onViewPatientImages()
{
    Patient patient = getSelectedPatient();
    if (patient.id <= 0) return;
    
    // 从数据库或文件系统获取该患者的影像路径列表
    QStringList imagePaths = getPatientImages(patient.id);
    
    if (imagePaths.isEmpty()) {
        QMessageBox::information(this, "提示", "该患者暂无医学影像");
        return;
    }
    
    // 打开影像查看器
    ImageViewerDialog dialog(this);
    dialog.loadImage(imagePaths.first());
    dialog.exec();
}
```

## 在主窗口添加菜单项

在 `mainwindow.cpp` 中添加：

```cpp
void MainWindow::setupMenuBar()
{
    QMenu *toolsMenu = menuBar()->addMenu("工具");
    
    QAction *imageViewerAction = toolsMenu->addAction("医学影像查看器");
    connect(imageViewerAction, &QAction::triggered, this, [this]() {
        ImageViewerDialog dialog(this);
        dialog.exec();
    });
}
```

## 扩展功能建议

### 1. DICOM 支持
当前代码对 DICOM 支持有限，建议集成 DCMTK 库：

```cpp
// 使用 DCMTK 读取 DICOM
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"

QImage loadDicom(const QString &filePath)
{
    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(filePath.toStdString().c_str());
    if (status.good()) {
        DicomImage *dcmImage = new DicomImage(&fileformat.getDataset());
        if (dcmImage != NULL && dcmImage->getStatus() == EIS_Normal) {
            // 转换为 QImage
        }
    }
}
```

### 2. 图像处理增强
可以添加更多医学图像处理功能：

```cpp
// 边缘检测（用于病灶识别）
cv::Mat detectEdges(const cv::Mat &input)
{
    cv::Mat edges;
    cv::Canny(input, edges, 50, 150);
    return edges;
}

// 图像分割
 cv::Mat segmentRegion(const cv::Mat &input, const cv::Rect &roi)
{
    cv::Mat mask = cv::Mat::zeros(input.size(), CV_8UC1);
    mask(roi) = 255;
    
    cv::Mat result;
    cv::grabCut(input, mask, roi, bgModel, fgModel, 5, cv::GC_INIT_WITH_RECT);
    return result;
}
```

### 3. 测量工具
添加医学测量功能：

```cpp
// 在 MedicalImageViewer 中添加
class MedicalImageViewer : public QWidget
{
    // ...
    
    void setPixelSpacing(double mmPerPixel);  // 设置像素间距
    double measureDistance(QPoint p1, QPoint p2);  // 测量距离（mm）
    double measureArea(const QPolygon &polygon);   // 测量面积（mm²）
    
private:
    double m_pixelSpacing = 1.0;  // 默认 1mm/像素
};
```

## 注意事项

1. **OpenCV 版本**：建议使用 OpenCV 4.x 版本
2. **运行时依赖**：Windows 下需要将 OpenCV 的 DLL 文件放在可执行文件目录或系统 PATH 中
3. **大图像处理**：对于超大医学影像（如 whole slide images），建议使用图像金字塔或分块加载
4. **DICOM 支持**：完整 DICOM 支持需要额外的 DCMTK 库

## 参考链接

- OpenCV 官方文档：https://docs.opencv.org/
- Qt 图像处理：https://doc.qt.io/qt-6/qimage.html
- DICOM 标准：https://www.dicomstandard.org/
