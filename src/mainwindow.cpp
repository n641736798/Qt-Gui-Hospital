#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplotchart_adapter.h"
#include "qtchart_adapter.h"
#include "qcustomplot.h"

#include <QPen>
#include <QPalette>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QSplitter>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMessageBox>
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

    // 样式已在 UI 文件中设置，无需重复设置

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
    loadAppConfig();
    setupDatabase();
    setupPatientsPage();
    setupMedicalRecordsPage();
    setupECGDashboard();
    setupECGThread();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Set initial page to home
    ui->contentStackWidget->setCurrentWidget(ui->homePage);
}

void MainWindow::loadAppConfig()
{
    // 默认配置：都使用 MVC 模式
    mvcConfig.patients = true;        // 患者管理默认使用 MVC
    mvcConfig.medicalRecords = true;   // 病历管理默认使用 MVC

    // 在若干典型路径中查找配置文件，适配不同运行目录
    QStringList candidatePaths;
    const QString appDirPath = QCoreApplication::applicationDirPath();
    QDir appDir(appDirPath);

    candidatePaths << appDir.filePath("config/app_config.json");
    candidatePaths << appDir.filePath("../config/app_config.json");
    candidatePaths << appDir.filePath("../../config/app_config.json");
    candidatePaths << appDir.filePath("../../../config/app_config.json");
    candidatePaths << appDir.filePath("../../../../config/app_config.json");
    candidatePaths << appDir.filePath("../../../../../config/app_config.json");
    candidatePaths << "D:/study/C++ code/Qt-Gui-Hospital-main/config/app_config.json";
    
    qDebug() << "[MainWindow] Application directory:" << appDirPath;
    qDebug() << "[MainWindow] Searching config files...";

    QString configPath;
    for (const QString &path : candidatePaths) {
        if (QFile::exists(path)) {
            configPath = path;
            break;
        }
    }

    if (configPath.isEmpty()) {
        qDebug() << "[MainWindow] app_config.json not found, using default MVC config.";
        return;
    }

    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[MainWindow] Failed to open app_config.json, using default MVC config:"
                 << file.errorString();
        return;
    }

    const QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "[MainWindow] Failed to parse app_config.json, using default MVC config:"
                 << parseError.errorString();
        return;
    }

    QJsonObject root = doc.object();
    if (root.contains("mvc") && root.value("mvc").isObject()) {
        QJsonObject mvcObj = root.value("mvc").toObject();

        if (mvcObj.contains("patients")) {
            mvcConfig.patients = mvcObj.value("patients").toBool(mvcConfig.patients);
        }
        if (mvcObj.contains("medical_records")) {
            mvcConfig.medicalRecords = mvcObj.value("medical_records").toBool(mvcConfig.medicalRecords);
        }
    }

    // 读取图表渲染器配置
    if (root.contains("chart_renderer")) {
        QString renderer = root.value("chart_renderer").toString().toLower();
        if (renderer == "qtchart") {
            m_chartRenderer = ChartRenderer::QtChart;
        } else {
            m_chartRenderer = ChartRenderer::QCustomPlot;
        }
    }

    QString rendererName = (m_chartRenderer == ChartRenderer::QCustomPlot) ? "QCustomPlot" : "QtChart";
    qDebug() << "[MainWindow] config loaded from" << configPath
             << "patients MVC =" << mvcConfig.patients
             << "medical_records MVC =" << mvcConfig.medicalRecords
             << "chart renderer =" << rendererName;

    // 显示配置加载成功的信息（可选）
    // QMessageBox::information(nullptr, "配置加载成功", 
    //     QString("配置文件: %1\n图表渲染器: %2").arg(configPath, rendererName));

    // 在状态栏显示当前模式状态
    QString statusText = QString("当前模式：患者管理(%1) | 病历管理(%2) | 图表渲染(%3)")
                            .arg(mvcConfig.patients ? "MVC" : "非MVC")
                            .arg(mvcConfig.medicalRecords ? "MVC" : "非MVC")
                            .arg(rendererName);
    statusBar()->showMessage(statusText);
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
    QWidget *patientsPage = ui->patientsPage;

    // 清理旧布局
    if (patientsPage->layout()) {
        delete patientsPage->layout();
    }

    // 根据配置决定使用 MVC 版本还是非 MVC 版本
    if (mvcConfig.patients) {
        // MVC：整个 patientsPage 只放一个 PatientWidget
        QVBoxLayout *layout = new QVBoxLayout(patientsPage);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        PatientWidget *widget = new PatientWidget(patientsPage);
        layout->addWidget(widget);

        // 非 MVC 相关成员在 MVC 模式下不使用
        patientTable = nullptr;
        searchEdit = nullptr;
        addPatientBtn = nullptr;
        editPatientBtn = nullptr;
        deletePatientBtn = nullptr;
    } else {
        // 非 MVC：保留原来的 QTableWidget 实现
        QVBoxLayout *mainLayout = new QVBoxLayout(patientsPage);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);

        // 非 MVC 版本患者管理标题
        QLabel *titleLabel = new QLabel("患者信息管理 (非 MVC 版本)");
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
}

void MainWindow::setupMedicalRecordsPage()
{
    int appointmentsIndex = ui->contentStackWidget->indexOf(ui->appointmentsPage);

    if (mvcConfig.medicalRecords) {
        medicalRecordWidget = new MedicalRecordWidget(this);
        medicalRecordsStackPage = medicalRecordWidget;
        ui->contentStackWidget->insertWidget(appointmentsIndex, medicalRecordsStackPage);
        connect(dbManager, &DatabaseManager::patientDataChanged,
                medicalRecordWidget, &MedicalRecordWidget::refreshData);
        connect(dbManager, &DatabaseManager::patientDeleted,
                medicalRecordWidget, &MedicalRecordWidget::refreshData);
    } else {
        QWidget *container = new QWidget(this);
        container->setStyleSheet("background-color: #F5E6D3;");
        QVBoxLayout *mainLayout = new QVBoxLayout(container);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);

        // 非 MVC 版本病历管理标题
        QLabel *titleLabel = new QLabel("病历管理 (非 MVC 版本)");
        titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; margin-bottom: 10px;");
        mainLayout->addWidget(titleLabel);

        QHBoxLayout *topLayout = new QHBoxLayout();
        QLabel *searchLabel = new QLabel("搜索:");
        searchLabel->setStyleSheet("color: #8B4513; font-weight: bold;");
        medicalRecordSearchEdit = new QLineEdit();
        medicalRecordSearchEdit->setPlaceholderText("按患者姓名、诊断或医生搜索...");
        medicalRecordSearchEdit->setStyleSheet(R"(
            QLineEdit {
                background-color: #FFFFFF;
                border: 1px solid #D2B48C;
                border-radius: 6px;
                padding: 8px;
                color: #8B4513;
                font-size: 14px;
            }
            QLineEdit:focus { border-color: #FF8C42; }
        )");
        connect(medicalRecordSearchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchMedicalRecords);

        topLayout->addWidget(searchLabel);
        topLayout->addWidget(medicalRecordSearchEdit);
        topLayout->addStretch();

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
            QPushButton:hover { background-color: #FF9F5A; }
            QPushButton:pressed { background-color: #E6793A; }
            QPushButton:disabled { background-color: #D2B48C; color: #CD853F; }
        )";

        addRecordBtn = new QPushButton("添加病历");
        editRecordBtn = new QPushButton("编辑病历");
        deleteRecordBtn = new QPushButton("删除病历");
        addRecordBtn->setStyleSheet(buttonStyle);
        editRecordBtn->setStyleSheet(buttonStyle);
        deleteRecordBtn->setStyleSheet(buttonStyle);
        editRecordBtn->setEnabled(false);
        deleteRecordBtn->setEnabled(false);

        connect(addRecordBtn, &QPushButton::clicked, this, &MainWindow::onAddRecordClicked);
        connect(editRecordBtn, &QPushButton::clicked, this, &MainWindow::onEditRecordClicked);
        connect(deleteRecordBtn, &QPushButton::clicked, this, &MainWindow::onDeleteRecordClicked);

        topLayout->addWidget(addRecordBtn);
        topLayout->addWidget(editRecordBtn);
        topLayout->addWidget(deleteRecordBtn);
        mainLayout->addLayout(topLayout);

        medicalRecordTable = new QTableWidget();
        medicalRecordTable->setColumnCount(5);
        medicalRecordTable->setHorizontalHeaderLabels(
            QStringList() << "病历ID" << "患者姓名" << "创建日期" << "诊断" << "医生");
        medicalRecordTable->setStyleSheet(R"(
            QTableWidget {
                background-color: #FFFFFF;
                color: #8B4513;
                gridline-color: #D2B48C;
                border: 1px solid #D2B48C;
                border-radius: 6px;
            }
            QTableWidget::item { padding: 8px; border-bottom: 1px solid #D2B48C; }
            QTableWidget::item:selected { background-color: #FF8C42; color: #FFFFFF; }
            QHeaderView::section {
                background-color: #E8D5C4;
                color: #8B4513;
                padding: 10px;
                border: 1px solid #D2B48C;
                font-weight: bold;
            }
        )");
        medicalRecordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        medicalRecordTable->setAlternatingRowColors(true);
        medicalRecordTable->horizontalHeader()->setStretchLastSection(true);
        medicalRecordTable->verticalHeader()->setVisible(false);

        connect(medicalRecordTable, &QTableWidget::itemSelectionChanged, [this]() {
            bool hasSelection = medicalRecordTable && !medicalRecordTable->selectedItems().isEmpty();
            if (editRecordBtn) editRecordBtn->setEnabled(hasSelection);
            if (deleteRecordBtn) deleteRecordBtn->setEnabled(hasSelection);
        });
        connect(medicalRecordTable, &QTableWidget::cellDoubleClicked, [this](int row, int) {
            Q_UNUSED(row)
            onEditRecordClicked();
        });

        mainLayout->addWidget(medicalRecordTable);

        medicalRecordsStackPage = container;
        ui->contentStackWidget->insertWidget(appointmentsIndex, medicalRecordsStackPage);

        connect(dbManager, &DatabaseManager::patientDataChanged,
                this, [this]() { refreshMedicalRecordTable(); });
        connect(dbManager, &DatabaseManager::patientDeleted,
                this, [this]() { refreshMedicalRecordTable(); });
        connect(dbManager, &DatabaseManager::medicalRecordDataChanged,
                this, [this]() { refreshMedicalRecordTable(); });

        refreshMedicalRecordTable();
    }
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

void MainWindow::populateMedicalRecordTable(const QList<MedicalRecord> &records)
{
    if (!medicalRecordTable) return;
    medicalRecordTable->setRowCount(records.size());
    for (int i = 0; i < records.size(); ++i) {
        const MedicalRecord &r = records[i];
        medicalRecordTable->setItem(i, 0, new QTableWidgetItem(QString::number(r.id)));
        medicalRecordTable->setItem(i, 1, new QTableWidgetItem(r.patientName));
        medicalRecordTable->setItem(i, 2, new QTableWidgetItem(r.createdDate.toString("yyyy-MM-dd hh:mm")));
        QString diag = r.diagnosis;
        if (diag.length() > 50) diag = diag.left(47) + "...";
        medicalRecordTable->setItem(i, 3, new QTableWidgetItem(diag));
        medicalRecordTable->setItem(i, 4, new QTableWidgetItem(r.doctorName));
    }
    medicalRecordTable->resizeColumnsToContents();
}

void MainWindow::refreshMedicalRecordTable()
{
    if (!dbManager->isConnected() || !medicalRecordTable) return;
    QList<MedicalRecord> records = dbManager->getAllMedicalRecords();
    populateMedicalRecordTable(records);
}

void MainWindow::onSearchMedicalRecords()
{
    if (!dbManager->isConnected() || !medicalRecordTable) return;
    QString term = medicalRecordSearchEdit ? medicalRecordSearchEdit->text().trimmed() : QString();
    QList<MedicalRecord> records = term.isEmpty()
        ? dbManager->getAllMedicalRecords()
        : dbManager->searchMedicalRecords(term);
    populateMedicalRecordTable(records);
}

void MainWindow::onAddRecordClicked()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Create, this);
    if (dialog.exec() == QDialog::Accepted) {
        MedicalRecord record = dialog.getMedicalRecord();
        if (dbManager->isConnected() && dbManager->addMedicalRecord(record)) {
            QMessageBox::information(this, "成功", "病历添加成功！");
            refreshMedicalRecordTable();
        } else {
            QMessageBox::critical(this, "错误", "添加病历失败。");
        }
    }
}

void MainWindow::onEditRecordClicked()
{
    if (!medicalRecordTable) return;
    int row = medicalRecordTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "编辑病历", "请先选择一条病历记录");
        return;
    }
    int recordId = medicalRecordTable->item(row, 0)->text().toInt();
    MedicalRecord record = dbManager->getMedicalRecordById(recordId);
    if (record.id <= 0) {
        QMessageBox::warning(this, "编辑病历", "无法获取病历信息");
        return;
    }
    MedicalRecordDialog dialog(MedicalRecordDialog::Edit, this);
    dialog.setMedicalRecord(record);
    if (dialog.exec() == QDialog::Accepted) {
        MedicalRecord updated = dialog.getMedicalRecord();
        if (dbManager->updateMedicalRecord(updated)) {
            QMessageBox::information(this, "成功", "病历更新成功！");
            refreshMedicalRecordTable();
        } else {
            QMessageBox::critical(this, "错误", "更新病历失败。");
        }
    }
}

void MainWindow::onDeleteRecordClicked()
{
    if (!medicalRecordTable) return;
    int row = medicalRecordTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "删除病历", "请先选择一条病历记录");
        return;
    }
    int recordId = medicalRecordTable->item(row, 0)->text().toInt();
    QString info = medicalRecordTable->item(row, 3)->text();
    if (QMessageBox::question(this, "确认删除",
            QString("确定要删除该病历记录吗？\n诊断：%1").arg(info),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;
    if (dbManager->deleteMedicalRecord(recordId)) {
        QMessageBox::information(this, "成功", "病历删除成功！");
        refreshMedicalRecordTable();
    } else {
        QMessageBox::critical(this, "错误", "删除病历失败。");
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

    // 创建图表渲染器标识标签
    QLabel *rendererLabel = new QLabel();
    QString rendererName = (m_chartRenderer == ChartRenderer::QCustomPlot) ? "QCustomPlot" : "QtChart";
    QString rendererColor = (m_chartRenderer == ChartRenderer::QCustomPlot) ? "#FF4444" : "#2C5F2D";
    rendererLabel->setText(QString("📊 当前图表渲染器: %1").arg(rendererName));
    rendererLabel->setStyleSheet(QString(
        "color: %1;"
        "font-size: 12pt;"
        "font-weight: bold;"
        "padding: 8px;"
        "background-color: #FFFFFF;"
        "border-radius: 6px;"
        "border: 2px solid %1;"
    ).arg(rendererColor));
    rendererLabel->setAlignment(Qt::AlignCenter);
    
    // 将渲染器标识添加到第一个框架的顶部
    ecgLayouts[0]->addWidget(rendererLabel);

    // 创建4通道心电图（QCustomPlot）
    QStringList channelNames;
    channelNames << "导联 I" << "导联 II" << "导联 III" << "导联 aVR";

    ecgCharts.clear();

    for (int i = 0; i < 4; ++i) {
        // 创建标题标签
        QLabel *titleLabel = new QLabel(channelNames[i]);
        titleLabel->setStyleSheet("color: #8B4513; font-size: 14pt; font-weight: bold; padding: 5px;");
        ecgLayouts[i]->addWidget(titleLabel);

        // 创建图表
        IECGChart *chart = createChart(channelNames[i], "幅值(mV)", -2, 2);
        ecgCharts.append(chart);

        // 添加到布局
        ecgLayouts[i]->addWidget(chart);
        chart->setMinimumHeight(150);
    }
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
    // 直接更新图表数据，不清除
    for (int i = 0; i < 4 && i < ecgCharts.size() && i < ecgDataBuffers.size(); ++i) {
        IECGChart *chart = ecgCharts[i];
        
        // 只添加新数据点，不清除旧数据
        // 注意：addDataPoint内部会管理缓冲区大小
        if (!ecgDataBuffers[i].isEmpty()) {
            // 清除并重新设置数据（适配器内部会管理）
            chart->clearData();
            for (const QPointF &point : ecgDataBuffers[i]) {
                chart->addDataPoint(point.x(), point.y());
            }
        }
    }

    axisUpdateCounter++;
    if (axisUpdateCounter >= 10) {
        axisUpdateCounter = 0;

        for (int i = 0; i < 4 && i < ecgCharts.size() && i < ecgDataBuffers.size(); ++i) {
            if (!ecgDataBuffers[i].isEmpty()) {
                IECGChart *chart = ecgCharts[i];
                double currentTime = ecgDataBuffers[i].last().x();
                double windowSize = 10.0;
                chart->setXRange(currentTime - windowSize, currentTime);
            }
        }
    }

    // 统一更新所有图表显示
    for (IECGChart *chart : ecgCharts) {
        chart->updateDisplay();
    }
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
    ui->contentStackWidget->setCurrentWidget(medicalRecordsStackPage);
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
// setChildrenBackground 函数已移除 - 样式现在由 UI 文件统一管理
