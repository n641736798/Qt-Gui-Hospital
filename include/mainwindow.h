#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartGlobal>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>
#include "databasemanager.h"
#include "patientdialog.h"
#include "ecgdatathread.h"
#include "medicalrecordwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onToggleButtonClicked();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void on_close_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_patientsButton_clicked();
    void on_homeButton_clicked();
    void on_appointmentsButton_clicked();
    void on_medicalRecordsButton_clicked();

    // Database and patient management slots
    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void onSearchPatients();
    void onPatientTableDoubleClicked(int row, int column);
    void refreshPatientTable();
    void updateDashboardWithRealData();
    
    // ECG data handling slots
    void onECGDataReceived(const ECGDataPoint &data);

private:
    Ui::MainWindow *ui;
    bool isSidebarOpen = true; // Sidebar starts open as per the UI file
    QPropertyAnimation *sidebarAnimation;

    void alignButtons();
    void setupECGDashboard();
    void setupPatientsPage();
    void setupMedicalRecordsPage();
    void setupDatabase();
    bool createDatabaseIfNotExists(const QString &host, const QString &username, const QString &password);
    void populatePatientTable(const QList<Patient> &patients);
    void setChildrenBackground(QWidget *widget, const QColor &color);

    bool dragging = false;
    QPoint dragPosition;

    // Database
    DatabaseManager *dbManager;
    
    // Patient management widgets
    QTableWidget *patientTable;
    QLineEdit *searchEdit;
    QPushButton *addPatientBtn;
    QPushButton *editPatientBtn;
    QPushButton *deletePatientBtn;
    
    // Medical record management widget
    MedicalRecordWidget *medicalRecordWidget;
    
    // ECG Dashboard
    ECGDataThread *ecgDataThread;
#ifndef NO_QT_CHARTS
    QList<QLineSeries*> ecgSeries;
    QList<QChartView*> ecgChartViews;
#endif
    QList<QList<QPointF>> ecgDataBuffers; // 为每个导联存储数据点
    static const int MAX_DATA_POINTS = 50; // 最大显示数据点数
    int axisUpdateCounter; // 坐标轴更新计数器
    QTimer *chartUpdateTimer; // 图表更新定时器
    void setupECGThread();
    void updateECGDisplay(const ECGDataPoint &data);
    void batchUpdateCharts(); // 批量更新图表
};

#endif // MAINWINDOW_H
