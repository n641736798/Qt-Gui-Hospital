#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
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

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void on_close_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_patientsButton_clicked();
    void on_homeButton_clicked();
    void on_appointmentsButton_clicked();

    // Database and patient management slots
    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void onSearchPatients();
    void onPatientTableDoubleClicked(int row, int column);
    void refreshPatientTable();
    void updateDashboardWithRealData();

private:
    Ui::MainWindow *ui;
    bool isSidebarOpen = true; // Sidebar starts open as per the UI file
    QPropertyAnimation *sidebarAnimation;

    void alignButtons();
    void setupECGDashboard();
    void setupPatientsPage();
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
    
    // ECG Dashboard
    QTimer *ecgTimer;
    QList<QLineSeries*> ecgSeries;
    QList<QChartView*> ecgChartViews;
    int ecgDataIndex;
    void updateECGData();
    double generateECGValue(double x, int channel);
};

#endif // MAINWINDOW_H