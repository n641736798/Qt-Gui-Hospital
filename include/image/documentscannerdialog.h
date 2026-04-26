#ifndef DOCUMENTSCANNERDIALOG_H
#define DOCUMENTSCANNERDIALOG_H

#include <QDialog>
#include <QImage>
#include <opencv2/opencv.hpp>

class QLabel;
class QPushButton;
class QScrollArea;
class QSlider;
class QComboBox;

class DocumentScannerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentScannerDialog(QWidget *parent = nullptr);
    ~DocumentScannerDialog();

private slots:
    void onOpenImage();
    void onSaveImage();
    void onAutoDetect();
    void onApplyTransform();
    void onReset();
    void onThresholdChanged(int value);

private:
    void setupUI();
    void setupConnections();
    void updateDisplay();
    void detectDocumentEdges();
    cv::Mat applyPerspectiveTransform(const cv::Mat &image, const std::vector<cv::Point> &corners);
    std::vector<cv::Point> orderPoints(const std::vector<cv::Point> &pts);

    // OpenCV 图像转换
    cv::Mat QImageToCvMat(const QImage &image);
    QImage CvMatToQImage(const cv::Mat &mat);

    // UI 控件
    QLabel *m_imageLabel;
    QScrollArea *m_scrollArea;
    QPushButton *m_openBtn;
    QPushButton *m_saveBtn;
    QPushButton *m_detectBtn;
    QPushButton *m_transformBtn;
    QPushButton *m_resetBtn;
    QSlider *m_thresholdSlider;
    QComboBox *m_outputSizeCombo;

    // 图像数据
    cv::Mat m_originalMat;
    cv::Mat m_displayMat;
    cv::Mat m_transformedMat;
    QImage m_displayImage;

    // 文档角点
    std::vector<cv::Point> m_documentCorners;
    bool m_cornersDetected = false;

    // 参数
    int m_cannyThreshold = 50;
};

#endif // DOCUMENTSCANNERDIALOG_H
