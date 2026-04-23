#include "medicalrecordwidget.h"
#include "ui_medicalrecordwidget.h"
#include "medicalrecorddialog.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

MedicalRecordWidget::MedicalRecordWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MedicalRecordWidget)
    , m_model(nullptr)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

MedicalRecordWidget::~MedicalRecordWidget()
{
    delete ui;
}

void MedicalRecordWidget::setupUI()
{
    // Get database connection from DatabaseManager
    DatabaseManager &dbManager = DatabaseManager::instance();
    Q_UNUSED(dbManager); // Mark as intentionally unused for now
    
    // Create and setup the medical record model
    m_model = new MedicalRecordModel(this);
    
    // 读取配置文件中的懒加载设置
    QString configPath = QCoreApplication::applicationDirPath() + "/../config/app_config.json";
    QFile configFile(configPath);
    if (!configFile.exists()) {
        configPath = "config/app_config.json";
        configFile.setFileName(configPath);
    }
    
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = configFile.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            QJsonObject rootObj = doc.object();
            QJsonObject lazyLoadingObj = rootObj.value("lazy_loading").toObject();
            bool lazyEnabled = lazyLoadingObj.value("enabled").toBool(true);
            m_model->setLazyLoadingEnabled(lazyEnabled);
        }
        configFile.close();
    } else {
        qDebug() << "无法打开配置文件:" << configFile.fileName() << "错误:" << configFile.errorString();
    }
    
    // 创建排序过滤代理模型
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterKeyColumn(-1); // 搜索所有列
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    
    ui->tableView->setModel(m_proxyModel);
    
    // Configure table view
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setAlternatingRowColors(true);
    
    // Set column widths
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(MedicalRecordModel::Id, 80);
    header->resizeSection(MedicalRecordModel::PatientName, 120);
    header->resizeSection(MedicalRecordModel::CreatedDate, 120);
    header->resizeSection(MedicalRecordModel::Diagnosis, 200);
    header->resizeSection(MedicalRecordModel::DoctorName, 100);
    
    // Hide patient ID column as we show patient name instead
    ui->tableView->hideColumn(MedicalRecordModel::PatientId);
    
    // Initially disable edit and delete buttons
    updateButtonStates();
    
    // Load initial data
    refreshData();
}

void MedicalRecordWidget::setupConnections()
{
    // Button connections
    connect(ui->addButton, &QPushButton::clicked, this, &MedicalRecordWidget::onAddRecord);
    connect(ui->editButton, &QPushButton::clicked, this, &MedicalRecordWidget::onEditRecord);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MedicalRecordWidget::onDeleteRecord);
    
    // Search connection
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MedicalRecordWidget::onSearchChanged);
    
    // Table view connections
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MedicalRecordWidget::onSelectionChanged);
    connect(ui->tableView, &QTableView::doubleClicked, this, &MedicalRecordWidget::onDoubleClicked);
    
    // Connect to DatabaseManager signals for automatic UI refresh
    DatabaseManager &dbManager = DatabaseManager::instance();
    connect(&dbManager, &DatabaseManager::medicalRecordDataChanged,
            this, &MedicalRecordWidget::onMedicalRecordDataChanged);
    connect(&dbManager, &DatabaseManager::patientDataChanged,
            this, &MedicalRecordWidget::onPatientDataChanged);
    connect(&dbManager, &DatabaseManager::patientDeleted,
            this, &MedicalRecordWidget::onPatientDataChanged);
}

void MedicalRecordWidget::refreshData()
{
    if (m_model) {
        m_model->refreshData();
        updateButtonStates();
        
        // Show message if no records found
        if (m_proxyModel->rowCount() == 0) {
            showMessage("暂无病历记录");
        } else {
            hideMessage();
        }
    }
}

void MedicalRecordWidget::onAddRecord()
{
    // Create dialog in Create mode
    MedicalRecordDialog dialog(MedicalRecordDialog::Create, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Get the medical record data from dialog
        MedicalRecord record = dialog.getMedicalRecord();
        
        // Insert the record using the model
        // The model will trigger DatabaseManager which emits signals
        // The signals will automatically refresh the UI
        if (m_model->insertMedicalRecord(record)) {
            // Success - show success message
            // No need to call refreshData() manually - it's handled by signal
            QMessageBox::information(this, "添加成功", "病历记录已成功添加");
        } else {
            // Error - show error message
            QMessageBox::critical(this, "添加失败", "添加病历记录时发生错误，请重试");
        }
    }
}

void MedicalRecordWidget::onEditRecord()
{
    // Get current selection
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "编辑病历", "请先选择要编辑的病历记录");
        return;
    }
    
    // Get the selected row (转换代理索引到源模型索引)
    QModelIndex sourceIndex = m_proxyModel->mapToSource(selectedIndexes.first());
    int row = sourceIndex.row();
    MedicalRecord record = m_model->getMedicalRecord(row);
    
    if (record.id <= 0) {
        QMessageBox::warning(this, "编辑病历", "无法获取病历记录信息");
        return;
    }
    
    // Create dialog in Edit mode
    MedicalRecordDialog dialog(MedicalRecordDialog::Edit, this);
    dialog.setMedicalRecord(record);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Get the updated medical record data from dialog
        MedicalRecord updatedRecord = dialog.getMedicalRecord();
        
        // Update the record using the model
        // The model will trigger DatabaseManager which emits signals
        // The signals will automatically refresh the UI
        if (m_model->updateMedicalRecord(record.id, updatedRecord)) {
            // Success - show success message
            // No need to call refreshData() manually - it's handled by signal
            QMessageBox::information(this, "编辑成功", "病历记录已成功更新");
        } else {
            // Error - show error message
            QMessageBox::critical(this, "编辑失败", "更新病历记录时发生错误，请重试");
        }
    }
}

void MedicalRecordWidget::onDeleteRecord()
{
    // Get current selection
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "删除病历", "请先选择要删除的病历记录");
        return;
    }
    
    // Get the selected row (转换代理索引到源模型索引)
    QModelIndex sourceIndex = m_proxyModel->mapToSource(selectedIndexes.first());
    int row = sourceIndex.row();
    MedicalRecord record = m_model->getMedicalRecord(row);
    
    if (record.id <= 0) {
        QMessageBox::warning(this, "删除病历", "无法获取病历记录信息");
        return;
    }
    
    // Show confirmation dialog with record details
    QString confirmMessage = QString(
        "确定要删除以下病历记录吗？\n\n"
        "病历ID: %1\n"
        "患者: %2\n"
        "创建日期: %3\n"
        "诊断: %4\n"
        "医生: %5\n\n"
        "此操作无法撤销！"
    ).arg(record.id)
     .arg(record.patientName)
     .arg(record.createdDate.toString("yyyy-MM-dd hh:mm"))
     .arg(record.diagnosis.length() > 50 ? record.diagnosis.left(47) + "..." : record.diagnosis)
     .arg(record.doctorName);
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认删除",
        confirmMessage,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Delete the record using the model
        // The model will trigger DatabaseManager which emits signals
        // The signals will automatically refresh the UI
        if (m_model->deleteMedicalRecord(record.id)) {
            // Success - show success message
            // No need to call refreshData() manually - it's handled by signal
            QMessageBox::information(this, "删除成功", "病历记录已成功删除");
        } else {
            // Error - show error message
            QMessageBox::critical(this, "删除失败", "删除病历记录时发生错误，请重试");
        }
    }
}

void MedicalRecordWidget::onSearchChanged(const QString &text)
{
    if (m_proxyModel) {
        // 使用代理模型进行过滤
        m_proxyModel->setFilterFixedString(text);
        
        // Show message if no search results found
        if (m_proxyModel->rowCount() == 0 && !text.isEmpty()) {
            showMessage("未找到匹配的病历记录");
        } else {
            hideMessage();
        }
    }
}

void MedicalRecordWidget::onSelectionChanged()
{
    updateButtonStates();
}

void MedicalRecordWidget::onDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    // Double-click should trigger edit action
    onEditRecord();
}

void MedicalRecordWidget::updateButtonStates()
{
    bool hasSelection = ui->tableView->selectionModel() && 
                       ui->tableView->selectionModel()->hasSelection();
    
    ui->editButton->setEnabled(hasSelection);
    ui->deleteButton->setEnabled(hasSelection);
}

void MedicalRecordWidget::showMessage(const QString &message)
{
    ui->messageLabel->setText(message);
    ui->messageLabel->setVisible(true);
    ui->tableView->setVisible(false);
}

void MedicalRecordWidget::hideMessage()
{
    ui->messageLabel->setVisible(false);
    ui->tableView->setVisible(true);
}

void MedicalRecordWidget::onMedicalRecordDataChanged()
{
    // Automatically refresh the UI when medical record data changes
    // This is triggered by DatabaseManager signals after CRUD operations
    if (m_model) {
        // 获取当前搜索过滤词
        QString currentFilter = ui->searchEdit->text();
        
        // Refresh the model data
        m_model->refreshData();
        
        // 更新按钮状态
        updateButtonStates();
        
        // Show appropriate message if no records
        if (m_proxyModel->rowCount() == 0) {
            if (!currentFilter.isEmpty()) {
                showMessage("未找到匹配的病历记录");
            } else {
                showMessage("暂无病历记录");
            }
        } else {
            hideMessage();
        }
    }
}

void MedicalRecordWidget::onPatientDataChanged()
{
    // Refresh UI when patient data changes (update or delete)
    // This ensures patient names are updated in medical records display
    if (m_model) {
        // Store current search filter and selection
        QString currentFilter = ui->searchEdit->text();
        QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
        int selectedSourceRow = -1;
        if (!selectedIndexes.isEmpty()) {
            // 保存源模型的选中行号
            QModelIndex sourceIndex = m_proxyModel->mapToSource(selectedIndexes.first());
            selectedSourceRow = sourceIndex.row();
        }
        
        // Refresh the model data
        m_model->refreshData();
        
        // Restore selection if possible
        if (selectedSourceRow >= 0 && selectedSourceRow < m_model->rowCount()) {
            // 转换源索引为代理索引并选中
            QModelIndex proxyIndex = m_proxyModel->mapFromSource(m_model->index(selectedSourceRow, 0));
            if (proxyIndex.isValid()) {
                ui->tableView->selectRow(proxyIndex.row());
            }
        }
        
        // Update button states
        updateButtonStates();
        
        // Show appropriate message if no records
        if (m_proxyModel->rowCount() == 0) {
            if (!currentFilter.isEmpty()) {
                showMessage("未找到匹配的病历记录");
            } else {
                showMessage("暂无病历记录");
            }
        } else {
            hideMessage();
        }
    }
}