#ifndef MORPHOLOGYDIALOG_H
#define MORPHOLOGYDIALOG_H

#include <QDialog>
#include <QImage>
#include <opencv2/opencv.hpp>

class QLabel;
class QPushButton;
class QScrollArea;
class QSlider;
class QComboBox;

class MorphologyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MorphologyDialog(QWidget *parent = nullptr);
    ~MorphologyDialog();

private slots:
    void onOpenImage();
    void onSaveImage();
    void onApplyErosion();
    void onApplyDilation();
    void onApplyOpening();
    void onApplyClosing();
    void onApplyGradient();
    void onReset();
    void onKernelSizeChanged(int value);
    void onIterationsChanged(int value);

private:
    void setupUI();
    void setupConnections();
    void updateDisplay();
    void applyMorphology(int operation);

    // OpenCV 图像转换
    cv::Mat QImageToCvMat(const QImage &image);
    QImage CvMatToQImage(const cv::Mat &mat);

    // UI 控件
    QLabel *m_imageLabel;
    QScrollArea *m_scrollArea;
    QPushButton *m_openBtn;
    QPushButton *m_saveBtn;
    QPushButton *m_erosionBtn;
    QPushButton *m_dilationBtn;
    QPushButton *m_openingBtn;
    QPushButton *m_closingBtn;
    QPushButton *m_gradientBtn;
    QPushButton *m_resetBtn;
    QSlider *m_kernelSizeSlider;
    QSlider *m_iterationsSlider;
    QLabel *m_kernelSizeLabel;
    QLabel *m_iterationsLabel;

    // 图像数据
    cv::Mat m_originalMat;
    cv::Mat m_processedMat;
    QImage m_displayImage;

    // 参数
    int m_kernelSize = 5;
    int m_iterations = 1;
};

#endif // MORPHOLOGYDIALOG_H
