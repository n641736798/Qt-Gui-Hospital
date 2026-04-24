#include "patientwidget.h"
#include "databasemanager.h"

#include <QHeaderView>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>

PatientWidget::PatientWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(new PatientModel(this))
    , m_proxyModel(new QSortFilterProxyModel(this))
{
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
    
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterKeyColumn(-1); // 搜索所有列
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    
    setupUI();
    setupConnections();
    refreshData();
}

void PatientWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(20);

    // 标题区域
    QLabel *titleLabel = new QLabel(QStringLiteral("患者信息管理"), this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700; color: #1E293B; margin-bottom: 4px;");
    mainLayout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel(QStringLiteral("管理医院患者的基本信息和病历记录"), this);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #64748B; margin-bottom: 16px;");
    mainLayout->addWidget(subtitleLabel);

    // 工具栏卡片
    QFrame *toolbarCard = new QFrame(this);
    toolbarCard->setStyleSheet(R"(
        QFrame {
            background-color: #FFFFFF;
            border-radius: 12px;
            border: 1px solid #E2E8F0;
        }
    )");
    QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbarCard);
    toolbarLayout->setContentsMargins(16, 16, 16, 16);
    toolbarLayout->setSpacing(12);

    // 搜索区域
    QLabel *searchIcon = new QLabel(this);
    searchIcon->setText("🔍");
    searchIcon->setStyleSheet("font-size: 16px;");
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(QStringLiteral("按姓名、电话或邮箱搜索患者..."));
    m_searchEdit->setMinimumWidth(300);
    m_searchEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #F8FAFC;
            border: 2px solid #E2E8F0;
            border-radius: 8px;
            padding: 10px 14px;
            color: #1E293B;
            font-size: 14px;
        }
        QLineEdit:hover {
            border-color: #CBD5E1;
        }
        QLineEdit:focus {
            border-color: #2563EB;
            background-color: #FFFFFF;
        }
        QLineEdit::placeholder {
            color: #94A3B8;
        }
    )");

    toolbarLayout->addWidget(searchIcon);
    toolbarLayout->addWidget(m_searchEdit);
    toolbarLayout->addStretch();

    // 按钮样式
    QString primaryButtonStyle = R"(
        QPushButton {
            background-color: #2563EB;
            color: #FFFFFF;
            border: none;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #3B82F6;
        }
        QPushButton:pressed {
            background-color: #1D4ED8;
        }
    )";
    
    QString secondaryButtonStyle = R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #374151;
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #F9FAFB;
            border-color: #9CA3AF;
        }
        QPushButton:pressed {
            background-color: #F3F4F6;
        }
        QPushButton:disabled {
            background-color: #F3F4F6;
            color: #9CA3AF;
            border-color: #E5E7EB;
        }
    )";
    
    QString dangerButtonStyle = R"(
        QPushButton {
            background-color: #EF4444;
            color: #FFFFFF;
            border: none;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #F87171;
        }
        QPushButton:pressed {
            background-color: #DC2626;
        }
        QPushButton:disabled {
            background-color: #FECACA;
            color: #FCA5A5;
        }
    )";

    m_addButton = new QPushButton(QStringLiteral("+ 添加患者"), this);
    m_editButton = new QPushButton(QStringLiteral("编辑"), this);
    m_deleteButton = new QPushButton(QStringLiteral("删除"), this);

    m_addButton->setStyleSheet(primaryButtonStyle);
    m_editButton->setStyleSheet(secondaryButtonStyle);
    m_deleteButton->setStyleSheet(dangerButtonStyle);

    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);

    toolbarLayout->addWidget(m_addButton);
    toolbarLayout->addWidget(m_editButton);
    toolbarLayout->addWidget(m_deleteButton);

    mainLayout->addWidget(toolbarCard);

    // 表格卡片
    QFrame *tableCard = new QFrame(this);
    tableCard->setStyleSheet(R"(
        QFrame {
            background-color: #FFFFFF;
            border-radius: 12px;
            border: 1px solid #E2E8F0;
        }
    )");
    QVBoxLayout *tableCardLayout = new QVBoxLayout(tableCard);
    tableCardLayout->setContentsMargins(0, 0, 0, 0);
    tableCardLayout->setSpacing(0);

    // 表格视图（MVC）
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setSortingEnabled(true);
    m_tableView->setShowGrid(false);
    m_tableView->verticalHeader()->setVisible(false);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->setHighlightSections(false);

    m_tableView->setStyleSheet(R"(
        QTableView {
            background-color: #FFFFFF;
            border: none;
            border-radius: 12px;
            color: #1E293B;
            selection-background-color: #DBEAFE;
            selection-color: #1E293B;
            alternate-background-color: #F8FAFC;
        }
        QTableView::item {
            padding: 14px 16px;
            border-bottom: 1px solid #F1F5F9;
        }
        QTableView::item:selected {
            background-color: #DBEAFE;
        }
        QTableView::item:hover {
            background-color: #F1F5F9;
        }
        QHeaderView::section {
            background-color: #F8FAFC;
            color: #475569;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #E2E8F0;
            font-weight: 700;
            font-size: 13px;
            text-transform: uppercase;
        }
        QHeaderView::section:hover {
            background-color: #F1F5F9;
        }
        QScrollBar:vertical {
            background-color: #F1F5F9;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background-color: #CBD5E1;
            min-height: 30px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #94A3B8;
        }
        QScrollBar::handle:vertical:pressed {
            background-color: #64748B;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background-color: #F1F5F9;
            height: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:horizontal {
            background-color: #CBD5E1;
            min-width: 30px;
            border-radius: 6px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #94A3B8;
        }
        QScrollBar::handle:horizontal:pressed {
            background-color: #64748B;
        }
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0px;
        }
    )");

    tableCardLayout->addWidget(m_tableView);
    mainLayout->addWidget(tableCard, 1);
}

void PatientWidget::setupConnections()
{
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &PatientWidget::onSearchTextChanged);
    connect(m_addButton, &QPushButton::clicked,
            this, &PatientWidget::onAddPatient);
    connect(m_editButton, &QPushButton::clicked,
            this, &PatientWidget::onEditPatient);
    connect(m_deleteButton, &QPushButton::clicked,
            this, &PatientWidget::onDeletePatient);

    connect(m_tableView, &QTableView::doubleClicked,
            this, &PatientWidget::onTableDoubleClicked);

    if (QItemSelectionModel *sel = m_tableView->selectionModel()) {
        connect(sel, &QItemSelectionModel::selectionChanged,
                this, &PatientWidget::onSelectionChanged);
    }
}

void PatientWidget::refreshData()
{
    if (m_model) {
        m_model->refresh();
    }
    updateButtonStates();
}

void PatientWidget::onSearchTextChanged(const QString &text)
{
    if (m_proxyModel) {
        m_proxyModel->setFilterFixedString(text);
    }
    updateButtonStates();
}

void PatientWidget::onAddPatient()
{
    PatientDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    Patient newPatient = dialog.getPatient();
    DatabaseManager &db = DatabaseManager::instance();
    if (!db.isConnected()) {
        QMessageBox::warning(this, QStringLiteral("数据库错误"), QStringLiteral("数据库连接不可用。"));
        return;
    }

    if (db.addPatient(newPatient)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("患者信息添加成功！"));
        refreshData();
    } else {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("添加患者信息到数据库失败。"));
    }
}

void PatientWidget::onEditPatient()
{
    if (!m_tableView) {
        return;
    }

    QModelIndex currentIndex = m_tableView->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    // 把代理模型的索引转换为源模型的索引
    QModelIndex sourceIndex = m_proxyModel->mapToSource(currentIndex);
    const int row = sourceIndex.row();
    Patient patient = m_model->getPatient(row);
    if (patient.id <= 0) {
        return;
    }

    PatientDialog dialog(patient, this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    Patient updatedPatient = dialog.getPatient();

    DatabaseManager &db = DatabaseManager::instance();
    if (db.updatePatient(updatedPatient)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("患者信息更新成功！"));
        refreshData();
    } else {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("更新患者信息失败。"));
    }
}

void PatientWidget::onDeletePatient()
{
    if (!m_tableView) {
        return;
    }

    QModelIndex currentIndex = m_tableView->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    // 把代理模型的索引转换为源模型的索引
    QModelIndex sourceIndex = m_proxyModel->mapToSource(currentIndex);
    const int row = sourceIndex.row();
    Patient patient = m_model->getPatient(row);
    if (patient.id <= 0) {
        return;
    }

    int ret = QMessageBox::question(
        this,
        QStringLiteral("确认删除"),
        QStringLiteral("您确定要删除患者 '%1' 吗？").arg(patient.name),
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret != QMessageBox::Yes) {
        return;
    }

    DatabaseManager &db = DatabaseManager::instance();
    if (db.deletePatient(patient.id)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("患者信息删除成功！"));
        refreshData();
    } else {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("删除患者信息失败。"));
    }
}

void PatientWidget::onTableDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    onEditPatient();
}

void PatientWidget::onSelectionChanged()
{
    updateButtonStates();
}

void PatientWidget::updateButtonStates()
{
    bool hasSelection = false;
    if (m_tableView && m_tableView->selectionModel()) {
        hasSelection = m_tableView->selectionModel()->hasSelection();
    }
    if (m_editButton) {
        m_editButton->setEnabled(hasSelection);
    }
    if (m_deleteButton) {
        m_deleteButton->setEnabled(hasSelection);
    }
}

