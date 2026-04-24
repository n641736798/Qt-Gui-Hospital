#ifndef IMAGEVIEWERDIALOG_H
#define IMAGEVIEWERDIALOG_H

#include <QDialog>

class MedicalImageViewer;
class QPushButton;
class QLabel;
class QLineEdit;

class ImageViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageViewerDialog(QWidget *parent = nullptr);
    ~ImageViewerDialog();

    // 加载图像
    bool loadImage(const QString &filePath);

private slots:
    void onOpenImage();
    void onSaveImage();
    void onImageLoaded(const QString &filePath);

private:
    void setupUI();
    void setupConnections();

    MedicalImageViewer *m_imageViewer;
    QPushButton *m_openBtn;
    QPushButton *m_saveBtn;
    QLabel *m_infoLabel;
    QLineEdit *m_patientIdEdit;
    QString m_currentFilePath;
};

#endif // IMAGEVIEWERDIALOG_H
