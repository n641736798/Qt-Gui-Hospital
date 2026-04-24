#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>
#include <QColor>
#include <QPoint>
#include "databasemanager.h"
#include "patientdialog.h"
#include "ecgdatathread.h"
#include "medicalrecordwidget.h"
#include "medicalrecorddialog.h"
#include "patientwidget.h"
#include "qcustomplot.h"
#include "iecgchart.h"

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

    // 病历管理（非 MVC）槽函数
    void onAddRecordClicked();
    void onEditRecordClicked();
    void onDeleteRecordClicked();
    void onSearchMedicalRecords();
    void refreshMedicalRecordTable();
    
    // ECG data handling slots
    void onECGDataReceived(const ECGDataPoint &data);

private:
    struct MvcConfig {
        bool patients = false;         // 默认：患者管理使用非 MVC（当前实现）
        bool medicalRecords = true;    // 默认：病历管理使用 MVC（当前实现）
    };

    enum class ChartRenderer {
        QCustomPlot,
        QtChart
    };

    Ui::MainWindow *ui;
    bool isSidebarOpen = true; // Sidebar starts open as per the UI file
    QPropertyAnimation *sidebarAnimation;

    void alignButtons();
    void loadAppConfig();
    void setupECGDashboard();
    void setupPatientsPage();
    void setupMedicalRecordsPage();
    void setupDatabase();
    bool createDatabaseIfNotExists(const QString &host, const QString &username, const QString &password);
    void populatePatientTable(const QList<Patient> &patients);
    void populateMedicalRecordTable(const QList<MedicalRecord> &records);

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

    // 病历管理：MVC 时为 MedicalRecordWidget，非 MVC 时为带 QTableWidget 的容器
    MedicalRecordWidget *medicalRecordWidget = nullptr;
    QWidget *medicalRecordsStackPage = nullptr;  // 栈中当前病历页（MVC 或非 MVC）

    // 病历管理（非 MVC）控件
    QTableWidget *medicalRecordTable = nullptr;
    QLineEdit *medicalRecordSearchEdit = nullptr;
    QPushButton *addRecordBtn = nullptr;
    QPushButton *editRecordBtn = nullptr;
    QPushButton *deleteRecordBtn = nullptr;
    
    // ECG Dashboard
    ECGDataThread *ecgDataThread;
    QList<IECGChart*> ecgCharts;
    QList<QList<QPointF>> ecgDataBuffers; // 为每个导联存储数据点
    static const int MAX_DATA_POINTS = 200; // 最大显示数据点数
    int axisUpdateCounter; // 坐标轴更新计数器
    QTimer *chartUpdateTimer; // 图表更新定时器
    void setupECGThread();
    void updateECGDisplay(const ECGDataPoint &data);
    void batchUpdateCharts(); // 批量更新图表
    IECGChart* createChart(const QString &title, const QString &yAxisLabel, double yMin, double yMax);

    MvcConfig mvcConfig;
    ChartRenderer m_chartRenderer = ChartRenderer::QCustomPlot; // 默认使用QCustomPlot
};

#endif // MAINWINDOW_H
