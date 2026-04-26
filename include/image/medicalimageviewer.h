#ifndef MEDICALIMAGEVIEWER_H
#define MEDICALIMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <opencv2/opencv.hpp>

class QLabel;
class QSlider;
class QPushButton;
class QScrollArea;

class MedicalImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalImageViewer(QWidget *parent = nullptr);
    ~MedicalImageViewer();

    // 加载图像
    bool loadImage(const QString &filePath);
    bool loadImage(const QImage &image);
    
    // 获取当前图像
    QImage currentImage() const;
    
    // 清空图像
    void clear();

public slots:
    // 图像调整
    void setBrightness(int value);
    void setContrast(int value);
    void setZoom(double factor);
    void resetAdjustments();
    
    // 图像操作
    void rotateLeft();
    void rotateRight();
    void flipHorizontal();
    void flipVertical();

    // OpenCV 图像增强
    void applyGaussianBlur();      // 高斯滤波去噪
    void applyMedianBlur();        // 中值滤波去噪
    void applyCLAHE();             // 自适应直方图均衡化
    void applyCannyEdge();         // Canny 边缘检测
    void applySobelEdge();         // Sobel 边缘检测
    void resetToOriginal();        // 重置为原始图像

    // 标注工具
    void enableAnnotation(bool enable);
    void clearAnnotations();

signals:
    void imageLoaded(const QString &filePath);
    void zoomChanged(double factor);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void setupUI();
    void setupConnections();
    void updateDisplay();
    void applyAdjustments();

    // OpenCV 图像处理
    cv::Mat QImageToCvMat(const QImage &image);
    QImage CvMatToQImage(const cv::Mat &mat);

    // 成员变量
    QLabel *m_imageLabel;
    QScrollArea *m_scrollArea;

    // 原始图像和处理后的图像
    cv::Mat m_originalMat;
    cv::Mat m_processedMat;
    QImage m_displayImage;

    // 调整参数
    int m_brightness = 0;
    int m_contrast = 100;
    double m_zoomFactor = 1.0;
    int m_rotation = 0;
    bool m_flipH = false;
    bool m_flipV = false;

    // 标注
    bool m_annotationEnabled = false;
    QList<QRect> m_annotations;
    QPoint m_annotationStart;
    bool m_isDrawing = false;

    // 拖拽平移
    bool m_isPanning = false;
    QPoint m_panStartPos;
    int m_panStartHValue = 0;
    int m_panStartVValue = 0;

    // UI 控件
    QSlider *m_brightnessSlider;
    QSlider *m_contrastSlider;
    QPushButton *m_zoomInBtn;
    QPushButton *m_zoomOutBtn;
    QPushButton *m_resetBtn;
    QPushButton *m_rotateLeftBtn;
    QPushButton *m_rotateRightBtn;
    QPushButton *m_annotationBtn;

    // 图像增强按钮
    QPushButton *m_gaussianBtn;
    QPushButton *m_medianBtn;
    QPushButton *m_claheBtn;
    QPushButton *m_cannyBtn;
    QPushButton *m_sobelBtn;
    QPushButton *m_resetOriginalBtn;

    // 标注工具按钮
    QPushButton *m_clearAnnotationBtn;
};

#endif // MEDICALIMAGEVIEWER_H
