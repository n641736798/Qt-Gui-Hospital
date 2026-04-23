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
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel(QStringLiteral("患者信息管理 (MVC 版本)"), this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    // 顶部：搜索 + 按钮
    QHBoxLayout *topLayout = new QHBoxLayout();

    QLabel *searchLabel = new QLabel(QStringLiteral("搜索:"), this);
    searchLabel->setStyleSheet("color: #8B4513; font-weight: bold;");
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(QStringLiteral("按姓名、电话或邮箱搜索..."));
    m_searchEdit->setStyleSheet(R"(
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

    topLayout->addWidget(searchLabel);
    topLayout->addWidget(m_searchEdit);
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

    m_addButton = new QPushButton(QStringLiteral("添加患者"), this);
    m_editButton = new QPushButton(QStringLiteral("编辑患者"), this);
    m_deleteButton = new QPushButton(QStringLiteral("删除患者"), this);

    m_addButton->setStyleSheet(buttonStyle);
    m_editButton->setStyleSheet(buttonStyle);
    m_deleteButton->setStyleSheet(buttonStyle);

    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);

    topLayout->addWidget(m_addButton);
    topLayout->addWidget(m_editButton);
    topLayout->addWidget(m_deleteButton);

    mainLayout->addLayout(topLayout);

    // 表格视图（MVC）
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setSortingEnabled(true); // 开启排序功能

    m_tableView->setStyleSheet(R"(
        QTableView {
            background-color: #FFFFFF;
            color: #8B4513;
            gridline-color: #D2B48C;
            border: 1px solid #D2B48C;
            border-radius: 6px;
        }
        QTableView::item {
            padding: 8px;
            border-bottom: 1px solid #D2B48C;
        }
        QTableView::item:selected {
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

    if (QHeaderView *header = m_tableView->horizontalHeader()) {
        header->setStretchLastSection(true);
    }
    if (QHeaderView *vHeader = m_tableView->verticalHeader()) {
        vHeader->setVisible(false);
    }

    mainLayout->addWidget(m_tableView);
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

