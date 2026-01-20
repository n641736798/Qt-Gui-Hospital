#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPen>
#include <QPalette>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbManager(&DatabaseManager::instance())
    , ecgDataThread(new ECGDataThread(this))
    , axisUpdateCounter(0)
    , chartUpdateTimer(new QTimer(this))
{
    ui->setupUi(this);

    // 初始化ECG数据缓冲区
    ecgDataBuffers.resize(4);
    for (int i = 0; i < 4; ++i) {
        ecgDataBuffers[i].reserve(MAX_DATA_POINTS);
    }

    // 设置主窗体背景色 - 强制应用
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #F5E6D3 !important;
        }
        QMainWindow::centralWidget {
            background-color: #F5E6D3 !important;
        }
        QWidget#homePage {
            background-color: #F5E6D3 !important;
        }
        QWidget#patientsPage {
            background-color: #F5E6D3 !important;
        }
        QWidget#appointmentsPage {
            background-color: #F5E6D3 !important;
        }
    )");
    
    // 同时设置调色板确保背景色生效
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#F5E6D3"));
    palette.setColor(QPalette::Base, QColor("#F5E6D3"));
    this->setPalette(palette);
    
    // 强制设置各个页面的背景色
    ui->homePage->setStyleSheet("background-color: #F5E6D3;");
    ui->patientsPage->setStyleSheet("background-color: #F5E6D3;");
    ui->appointmentsPage->setStyleSheet("background-color: #F5E6D3;");
    ui->contentStackWidget->setStyleSheet("background-color: #F5E6D3;");

    // Sidebar animation setup
    sidebarAnimation = new QPropertyAnimation(ui->sidebar, "minimumWidth", this);
    sidebarAnimation->setDuration(200);

    connect(ui->toggleButton, &QPushButton::clicked, this, &MainWindow::onToggleButtonClicked);
    
    // 连接心电图控制按钮
    connect(ui->op1Button, &QPushButton::clicked, [this]() {
        ecgDataThread->startGeneration();
    });
    connect(ui->op2Button, &QPushButton::clicked, [this]() {
        ecgDataThread->stopGeneration();
    });

    alignButtons();
    setupDatabase();
    setupPatientsPage();
    setupMedicalRecordsPage();
    setupECGDashboard();
    setupECGThread();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Set initial page to home
    ui->contentStackWidget->setCurrentWidget(ui->homePage);
    
    // 最后的背景色强制设置 - 确保所有组件都是暖色
    QTimer::singleShot(100, this, [this]() {
        // 递归设置所有子组件的背景色
        setChildrenBackground(this, QColor("#F5E6D3"));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDatabase()
{
    // Database connection parameters
    QString host = "localhost";
    QString database = "hospital_db";
    QString username = "root";
    QString password = "123456";
    
    if (dbManager->connectToDatabase(host, database, username, password)) {
        qDebug() << "Database connected successfully";
        updateDashboardWithRealData();
    } else {
        // Try to create database if connection failed
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
            "Database Connection Failed", 
            "无法连接到数据库 'hospital_db'。\n\n"
            "可能的原因：\n"
            "1. 数据库 'hospital_db' 不存在\n"
            "2. MySQL 服务未启动\n"
            "3. 用户名或密码错误\n\n"
            "是否尝试创建数据库？",
            QMessageBox::Yes | QMessageBox::No);
            
        if (reply == QMessageBox::Yes) {
            // Try to connect without specifying database to create it
            if (createDatabaseIfNotExists(host, username, password)) {
                // Retry connection
                if (dbManager->connectToDatabase(host, database, username, password)) {
                    qDebug() << "Database created and connected successfully";
                    updateDashboardWithRealData();
                    QMessageBox::information(this, "成功", "数据库创建成功并已连接！");
                } else {
                    QMessageBox::critical(this, "错误", "数据库创建成功但连接失败。");
                }
            } else {
                QMessageBox::critical(this, "错误", 
                    "无法创建数据库。请检查：\n"
                    "1. MySQL 服务是否正在运行\n"
                    "2. 用户名密码是否正确\n"
                    "3. 用户是否有创建数据库的权限");
            }
        }
    }
}

void MainWindow::setupPatientsPage()
{
    // Get the patients page widget
    QWidget *patientsPage = ui->patientsPage;
    
    // Clear existing layout if any
    if (patientsPage->layout()) {
        delete patientsPage->layout();
    }
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(patientsPage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // Title
    QLabel *titleLabel = new QLabel("患者信息管理");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Search and buttons layout
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    // Search
    QLabel *searchLabel = new QLabel("搜索:");
    searchLabel->setStyleSheet("color: #8B4513; font-weight: bold;");
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("按姓名、电话或邮箱搜索...");
    searchEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #FFFFFF;
            border: 1px solid #D2B48C;
            border-radius: 6px;
            padding: 8px;
            color: #8B4513;
            font-size: 14px;
        }
        QLineEdit:focus {
            border-color: #FF8C42;
        }
    )");
    
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchPatients);
    
    topLayout->addWidget(searchLabel);
    topLayout->addWidget(searchEdit);
    topLayout->addStretch();
    
    // Buttons
    addPatientBtn = new QPushButton("添加患者");
    editPatientBtn = new QPushButton("编辑患者");
    deletePatientBtn = new QPushButton("删除患者");
    
    QString buttonStyle = R"(
        QPushButton {
            background-color: #FF8C42;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #FF9F5A;
        }
        QPushButton:pressed {
            background-color: #E6793A;
        }
        QPushButton:disabled {
            background-color: #D2B48C;
            color: #CD853F;
        }
    )";
    
    addPatientBtn->setStyleSheet(buttonStyle);
    editPatientBtn->setStyleSheet(buttonStyle);
    deletePatientBtn->setStyleSheet(buttonStyle);
    
    editPatientBtn->setEnabled(false);
    deletePatientBtn->setEnabled(false);
    
    connect(addPatientBtn, &QPushButton::clicked, this, &MainWindow::onAddPatientClicked);
    connect(editPatientBtn, &QPushButton::clicked, this, &MainWindow::onEditPatientClicked);
    connect(deletePatientBtn, &QPushButton::clicked, this, &MainWindow::onDeletePatientClicked);
    
    topLayout->addWidget(addPatientBtn);
    topLayout->addWidget(editPatientBtn);
    topLayout->addWidget(deletePatientBtn);
    
    mainLayout->addLayout(topLayout);
    
    // Patient table
    patientTable = new QTableWidget();
    patientTable->setColumnCount(7);
    QStringList headers;
    headers << "编号" << "姓名" << "电话" << "邮箱" << "出生日期" << "性别" << "病史";
    patientTable->setHorizontalHeaderLabels(headers);
    
    patientTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #FFFFFF;
            color: #8B4513;
            gridline-color: #D2B48C;
            border: 1px solid #D2B48C;
            border-radius: 6px;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #D2B48C;
        }
        QTableWidget::item:selected {
            background-color: #FF8C42;
            color: #FFFFFF;
        }
        QHeaderView::section {
            background-color: #E8D5C4;
            color: #8B4513;
            padding: 10px;
            border: 1px solid #D2B48C;
            font-weight: bold;
        }
    )");
    
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    patientTable->setAlternatingRowColors(true);
    patientTable->horizontalHeader()->setStretchLastSection(true);
    patientTable->verticalHeader()->setVisible(false);
    
    connect(patientTable, &QTableWidget::itemSelectionChanged, [this]() {
        bool hasSelection = !patientTable->selectedItems().isEmpty();
        editPatientBtn->setEnabled(hasSelection);
        deletePatientBtn->setEnabled(hasSelection);
    });
    
    connect(patientTable, &QTableWidget::cellDoubleClicked, 
            this, &MainWindow::onPatientTableDoubleClicked);
    
    mainLayout->addWidget(patientTable);
    
    // Load initial data
    refreshPatientTable();
}

void MainWindow::setupMedicalRecordsPage()
{
    // Create the medical record widget
    medicalRecordWidget = new MedicalRecordWidget(this);
    
    // Add it to the stacked widget between patients and appointments
    // Find the index of appointmentsPage
    int appointmentsIndex = ui->contentStackWidget->indexOf(ui->appointmentsPage);
    
    // Insert the medical records page before appointments
    ui->contentStackWidget->insertWidget(appointmentsIndex, medicalRecordWidget);
    
    // Connect database signals to refresh medical records when patient data changes
    connect(dbManager, &DatabaseManager::patientDataChanged,
            medicalRecordWidget, &MedicalRecordWidget::refreshData);
    connect(dbManager, &DatabaseManager::patientDeleted,
            medicalRecordWidget, &MedicalRecordWidget::refreshData);
}

void MainWindow::refreshPatientTable()
{
    if (!dbManager->isConnected()) {
        return;
    }
    
    QList<Patient> patients = dbManager->getAllPatients();
    populatePatientTable(patients);
}

void MainWindow::populatePatientTable(const QList<Patient> &patients)
{
    patientTable->setRowCount(patients.size());
    
    for (int i = 0; i < patients.size(); ++i) {
        const Patient &patient = patients[i];
        
        patientTable->setItem(i, 0, new QTableWidgetItem(QString::number(patient.id)));
        patientTable->setItem(i, 1, new QTableWidgetItem(patient.name));
        patientTable->setItem(i, 2, new QTableWidgetItem(patient.phone));
        patientTable->setItem(i, 3, new QTableWidgetItem(patient.email));
        patientTable->setItem(i, 4, new QTableWidgetItem(patient.birthDate));
        patientTable->setItem(i, 5, new QTableWidgetItem(patient.gender));
        
        QString shortHistory = patient.medicalHistory;
        if (shortHistory.length() > 50) {
            shortHistory = shortHistory.left(47) + "...";
        }
        patientTable->setItem(i, 6, new QTableWidgetItem(shortHistory));
    }
    
    // Resize columns to content
    patientTable->resizeColumnsToContents();
}

void MainWindow::onAddPatientClicked()
{
    PatientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Patient newPatient = dialog.getPatient();
        
        if (dbManager->isConnected()) {
            if (dbManager->addPatient(newPatient)) {
                QMessageBox::information(this, "成功", "患者信息添加成功！");
                refreshPatientTable();
                updateDashboardWithRealData();
            } else {
                QMessageBox::critical(this, "错误", "添加患者信息到数据库失败。");
            }
        } else {
            QMessageBox::warning(this, "数据库错误", "数据库连接不可用。");
        }
    }
}

void MainWindow::onEditPatientClicked()
{
    int currentRow = patientTable->currentRow();
    if (currentRow < 0) return;
    
    int patientId = patientTable->item(currentRow, 0)->text().toInt();
    Patient patient = dbManager->getPatientById(patientId);
    
    PatientDialog dialog(patient, this);
    if (dialog.exec() == QDialog::Accepted) {
        Patient updatedPatient = dialog.getPatient();
        
        if (dbManager->updatePatient(updatedPatient)) {
            QMessageBox::information(this, "成功", "患者信息更新成功！");
            refreshPatientTable();
        } else {
            QMessageBox::critical(this, "错误", "更新患者信息失败。");
        }
    }
}

void MainWindow::onDeletePatientClicked()
{
    int currentRow = patientTable->currentRow();
    if (currentRow < 0) return;
    
    QString patientName = patientTable->item(currentRow, 1)->text();
    int patientId = patientTable->item(currentRow, 0)->text().toInt();
    
    int ret = QMessageBox::question(this, "确认删除", 
        QString("您确定要删除患者 '%1' 吗？").arg(patientName),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (dbManager->deletePatient(patientId)) {
            QMessageBox::information(this, "成功", "患者信息删除成功！");
            refreshPatientTable();
            updateDashboardWithRealData();
        } else {
            QMessageBox::critical(this, "错误", "删除患者信息失败。");
        }
    }
}

void MainWindow::onSearchPatients()
{
    QString searchTerm = searchEdit->text().trimmed();
    
    if (!dbManager->isConnected()) {
        return;
    }
    
    QList<Patient> patients;
    if (searchTerm.isEmpty()) {
        patients = dbManager->getAllPatients();
    } else {
        patients = dbManager->searchPatients(searchTerm);
    }
    
    populatePatientTable(patients);
}

void MainWindow::onPatientTableDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    
    if (row >= 0) {
        onEditPatientClicked();
    }
}

void MainWindow::updateDashboardWithRealData()
{
    if (!dbManager->isConnected()) {
        return;
    }
    
    // Update charts with real data from database
    // This would replace the hardcoded data in the existing chart methods
    // For now, we'll keep the existing demo charts
    // You can extend this to use real data from dbManager->getPatientsByMonth(), etc.
}

void MainWindow::setupECGDashboard()
{
    // 清除原有的图表布局内容
    QLayoutItem *item;
    while ((item = ui->mainChartLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    while ((item = ui->barChartLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    while ((item = ui->verticalLayout_5->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    while ((item = ui->verticalLayout_6->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // 清除整个statsGrid布局中的所有内容
    QGridLayout *statsGrid = ui->statsGrid;
    while (statsGrid->count() > 0) {
        QLayoutItem *gridItem = statsGrid->takeAt(0);
        if (gridItem->widget()) {
            delete gridItem->widget();
        }
        delete gridItem;
    }

    // 重新创建4个心电图框架
    QList<QFrame*> ecgFrames;
    QList<QVBoxLayout*> ecgLayouts;
    
    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame();
        frame->setStyleSheet("background-color: #FFFFFF; border-radius: 12px; border: 1px solid #D2B48C;");
        
        QVBoxLayout *layout = new QVBoxLayout(frame);
        ecgLayouts.append(layout);
        ecgFrames.append(frame);
    }
    
    // 将框架添加到网格布局
    statsGrid->addWidget(ecgFrames[0], 0, 0, 1, 2); // 左上，跨2列
    statsGrid->addWidget(ecgFrames[1], 0, 2, 1, 2); // 右上，跨2列
    statsGrid->addWidget(ecgFrames[2], 1, 0, 1, 2); // 左下，跨2列
    statsGrid->addWidget(ecgFrames[3], 1, 2, 1, 2); // 右下，跨2列

    // 创建4通道心电图
    QStringList channelNames;
    channelNames << "导联 I" << "导联 II" << "导联 III" << "导联 aVR";
    
#ifndef NO_QT_CHARTS
    // 清空之前的系列和视图
    ecgSeries.clear();
    ecgChartViews.clear();
    
    for (int i = 0; i < 4; ++i) {
        // 创建标题标签
        QLabel *titleLabel = new QLabel(channelNames[i]);
        titleLabel->setStyleSheet("color: #8B4513; font-size: 14pt; font-weight: bold; padding: 5px;");
        ecgLayouts[i]->addWidget(titleLabel);
        
        // 创建心电图数据系列
        QLineSeries *series = new QLineSeries();
        ecgSeries.append(series);
        
        // 创建图表
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->legend()->hide();
        chart->setTitle("");
        chart->setBackgroundBrush(QBrush(QColor("#FFFFFF")));
        chart->setMargins(QMargins(0, 0, 0, 0));
        
        // 设置心电图线条样式
        QPen seriesPen(QColor("#FF4444")); // 红色心电图线
        seriesPen.setWidth(2);
        series->setPen(seriesPen);
        
        // 设置坐标轴
        QValueAxis *axisX = new QValueAxis();
        axisX->setRange(0, 10);
        axisX->setGridLineVisible(true);
        axisX->setGridLineColor(QColor("#E0E0E0"));
        axisX->setLabelsVisible(false);
        axisX->setLineVisible(false);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(-2, 2);
        axisY->setGridLineVisible(true);
        axisY->setGridLineColor(QColor("#E0E0E0"));
        axisY->setLabelsVisible(false);
        axisY->setLineVisible(false);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
        
        // 创建图表视图
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumHeight(150);
        ecgChartViews.append(chartView);
        
        ecgLayouts[i]->addWidget(chartView);
    }
#else
    // Qt Charts disabled - show placeholder labels
    for (int i = 0; i < 4; ++i) {
        QLabel *titleLabel = new QLabel(channelNames[i]);
        titleLabel->setStyleSheet("color: #8B4513; font-size: 14pt; font-weight: bold; padding: 5px;");
        ecgLayouts[i]->addWidget(titleLabel);
        
        QLabel *placeholder = new QLabel("ECG Chart (Qt Charts module disabled)");
        placeholder->setAlignment(Qt::AlignCenter);
        placeholder->setStyleSheet("color: #999; font-size: 12pt; padding: 50px;");
        placeholder->setMinimumHeight(150);
        ecgLayouts[i]->addWidget(placeholder);
    }
#endif
}

void MainWindow::setupECGThread()
{
    // 连接ECG数据线程的信号
    connect(ecgDataThread, &ECGDataThread::dataReady, 
            this, &MainWindow::onECGDataReceived, Qt::QueuedConnection);
    
    // 设置批量图表更新定时器
    chartUpdateTimer->setInterval(100); // 每100ms批量更新一次图表
    connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::batchUpdateCharts);
    chartUpdateTimer->start();
    
    // 设置默认参数
    ecgDataThread->setHeartRate(72);    // 72 BPM
    ecgDataThread->setAmplitude(1.0);   // 标准振幅
    ecgDataThread->setNoiseLevel(0.05); // 轻微噪声
    
    // 自动启动ECG数据生成
    ecgDataThread->startGeneration();
}

void MainWindow::onECGDataReceived(const ECGDataPoint &data)
{
    updateECGDisplay(data);
}

void MainWindow::updateECGDisplay(const ECGDataPoint &data)
{
    // 只更新数据缓冲区，不立即更新图表
    for (int i = 0; i < 4 && i < ecgDataBuffers.size(); ++i) {
        // 管理缓冲区大小
        if (ecgDataBuffers[i].size() >= MAX_DATA_POINTS) {
            ecgDataBuffers[i].removeFirst();
        }
        
        // 添加新数据点到缓冲区
        ecgDataBuffers[i].append(QPointF(data.timestamp, data.values[i]));
    }
    
    // Debug信息：输出缓冲区容量（降低频率）
    static int debugCounter = 0;
    debugCounter++;
    if (debugCounter % 100 == 0) { // 每100次更新输出一次
        qDebug() << "=== ECG Buffer Status (Update #" << debugCounter << ") ===";
        for (int i = 0; i < 4 && i < ecgDataBuffers.size(); ++i) {
            qDebug() << QString("导联 %1: 缓冲区大小 = %2/%3, 时间戳 = %4")
                        .arg(i + 1)
                        .arg(ecgDataBuffers[i].size())
                        .arg(MAX_DATA_POINTS)
                        .arg(data.timestamp, 0, 'f', 3);
        }
    }
}

void MainWindow::batchUpdateCharts()
{
#ifndef NO_QT_CHARTS
    // 批量更新所有图表
    for (int i = 0; i < 4 && i < ecgSeries.size(); ++i) {
        QLineSeries *series = ecgSeries[i];
        
        if (!ecgDataBuffers[i].isEmpty()) {
            // 只有当缓冲区有数据时才更新
            series->replace(ecgDataBuffers[i]);
        }
    }
    
    // 减少坐标轴更新频率
    axisUpdateCounter++;
    if (axisUpdateCounter >= 10) { // 每1秒更新一次坐标轴
        axisUpdateCounter = 0;
        
        // 更新X轴范围以实现滚动效果
        for (int i = 0; i < 4 && i < ecgChartViews.size(); ++i) {
            if (!ecgDataBuffers[i].isEmpty()) {
                QChart *chart = ecgChartViews[i]->chart();
                QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
                if (axisX) {
                    double currentTime = ecgDataBuffers[i].last().x();
                    double windowSize = 10.0; // 10秒窗口
                    axisX->setRange(currentTime - windowSize, currentTime);
                }
            }
        }
    }
#endif // NO_QT_CHARTS
}


void MainWindow::alignButtons()
{
    for (int i = 0; i < ui->sidebarLayout->count(); ++i) {
        QLayoutItem *item = ui->sidebarLayout->itemAt(i);
        if (item && item->widget()) {
            QWidget *widget = item->widget();
            if (QPushButton *button = qobject_cast<QPushButton*>(widget)) {
                button->setLayoutDirection(Qt::LeftToRight);
            }
        }
    }
}


void MainWindow::onToggleButtonClicked()
{
    int endWidth = isSidebarOpen ? 60 : 200;

    // 根据侧边栏状态设置按钮文字显示
    QList<QPushButton*> buttons;
    buttons << ui->toggleButton
            << ui->homeButton
            << ui->patientsButton
            << ui->medicalRecordsButton
            << ui->appointmentsButton
            << ui->settingsButton;
    
    for (QPushButton* button : buttons) {
        if (!button) continue;
        
        if (isSidebarOpen) {
            // 缩进时只显示图标，隐藏文字
            button->setText("");
        } else {
            // 展开时显示完整文字
            if (button == ui->toggleButton) {
                button->setText("医院系统");
            } else if (button == ui->homeButton) {
                button->setText("仪表盘");
            } else if (button == ui->patientsButton) {
                button->setText("患者管理");
            } else if (button == ui->medicalRecordsButton) {
                button->setText("病历管理");
            } else if (button == ui->appointmentsButton) {
                button->setText("预约管理");
            } else if (button == ui->settingsButton) {
                button->setText("系统设置");
            }
        }
    }

    // Set the end value for the animation and start it
    sidebarAnimation->setEndValue(endWidth);
    sidebarAnimation->start();

    // Toggle the state for the next click
    isSidebarOpen = !isSidebarOpen;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (ui->topbar->geometry().contains(event->pos())) {
            dragging = true;
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    dragging = false;
}

void MainWindow::on_close_clicked()
{
    QApplication::closeAllWindows();
}


void MainWindow::on_pushButton_2_clicked()
{
    if(isMaximized())
    {
        showNormal();
    }
    else
    {
        showMaximized();
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    showMinimized();
}


void MainWindow::on_patientsButton_clicked()
{
    ui->contentStackWidget->setCurrentWidget(ui->patientsPage);
}


void MainWindow::on_homeButton_clicked()
{
    ui->contentStackWidget->setCurrentWidget(ui->homePage);
}


void MainWindow::on_appointmentsButton_clicked()
{
    ui->contentStackWidget->setCurrentWidget(ui->appointmentsPage);
}

void MainWindow::on_medicalRecordsButton_clicked()
{
    ui->contentStackWidget->setCurrentWidget(medicalRecordWidget);
}

bool MainWindow::createDatabaseIfNotExists(const QString &host, const QString &username, const QString &password)
{
    // Create a temporary database connection without specifying a database
    QSqlDatabase tempDb = QSqlDatabase::addDatabase("QMYSQL", "temp_connection");
    tempDb.setHostName(host);
    tempDb.setUserName(username);
    tempDb.setPassword(password);
    tempDb.setPort(3306);
    
    if (!tempDb.open()) {
        qDebug() << "Failed to connect to MySQL server:" << tempDb.lastError().text();
        QSqlDatabase::removeDatabase("temp_connection");
        return false;
    }
    
    // Create the database
    QSqlQuery query(tempDb);
    bool success = query.exec("CREATE DATABASE IF NOT EXISTS hospital_db");
    
    if (!success) {
        qDebug() << "Failed to create database:" << query.lastError().text();
    }
    
    tempDb.close();
    QSqlDatabase::removeDatabase("temp_connection");
    
    return success;
}
void MainWindow::setChildrenBackground(QWidget *widget, const QColor &color)
{
    if (!widget) return;
    
    // 设置当前组件的背景色
    QString colorStr = color.name();
    widget->setStyleSheet(QString("background-color: %1;").arg(colorStr));
    
    // 递归设置所有子组件
    QList<QWidget*> children = widget->findChildren<QWidget*>();
    for (QWidget *child : children) {
        // 跳过图表组件，避免影响图表显示
#ifndef NO_QT_CHARTS
        if (child->inherits("QChartView") || child->inherits("QGraphicsView")) {
            continue;
        }
#else
        if (child->inherits("QGraphicsView")) {
            continue;
        }
#endif
        
        // 跳过已经有白色背景的卡片
        if (child->objectName().contains("Frame") && 
            child->styleSheet().contains("background-color: #FFFFFF")) {
            continue;
        }
        
        child->setStyleSheet(QString("background-color: %1;").arg(colorStr));
    }
}
