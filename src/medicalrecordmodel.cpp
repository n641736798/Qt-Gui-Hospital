#include "medicalrecordmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>

MedicalRecordModel::MedicalRecordModel(QObject *parent, QSqlDatabase db)
    : QAbstractTableModel(parent)
    , m_database(db)
    , m_dbManager(&DatabaseManager::instance())
    , m_currentPage(0)
    , m_hasMore(true)
    , m_totalCount(0)
{
    if (!m_database.isValid()) {
        m_database = QSqlDatabase::database();
    }
    
    setupModel();
}

int MedicalRecordModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_records.size();
}

bool MedicalRecordModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_lazyLoadingEnabled)
        return false;
    return m_hasMore;
}

void MedicalRecordModel::setLazyLoadingEnabled(bool enabled)
{
    m_lazyLoadingEnabled = enabled;
    if (!enabled) {
        // 关闭懒加载时，已经加载的数据保持不变，后续不再自动加载更多
        m_hasMore = false;
    }
}

bool MedicalRecordModel::isLazyLoadingEnabled() const
{
    return m_lazyLoadingEnabled;
}

void MedicalRecordModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid() || !m_hasMore)
        return;

    QList<MedicalRecord> newRecords = m_dbManager->getMedicalRecordsByPage(m_currentPage, PAGE_SIZE);

    if (newRecords.isEmpty()) {
        m_hasMore = false;
        return;
    }

    beginInsertRows(QModelIndex(), m_records.size(), m_records.size() + newRecords.size() - 1);
    m_records.append(newRecords);
    endInsertRows();

    m_currentPage++;
    if (newRecords.size() < PAGE_SIZE) {
        m_hasMore = false;
    }
}

void MedicalRecordModel::setupModel()
{
    refreshData();
}

QVariant MedicalRecordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_records.size()) {
        return QVariant();
    }

    const MedicalRecord &record = m_records.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Id: return record.id;
        case PatientId: return record.patientId;
        case PatientName: return record.patientName;
        case CreatedDate: return record.createdDate.toString("yyyy-MM-dd hh:mm");
        case Diagnosis: {
            QString text = record.diagnosis;
            if (text.length() > 50) {
                return text.left(47) + "...";
            }
            return text;
        }
        case Symptoms: {
            QString text = record.symptoms;
            if (text.length() > 50) {
                return text.left(47) + "...";
            }
            return text;
        }
        case Treatment: {
            QString text = record.treatment;
            if (text.length() > 50) {
                return text.left(47) + "...";
            }
            return text;
        }
        case DoctorName: return record.doctorName;
        case Notes: {
            QString text = record.notes;
            if (text.length() > 50) {
                return text.left(47) + "...";
            }
            return text;
        }
        default: break;
        }
    }

    return QVariant();
}

QVariant MedicalRecordModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case Id: return "病历ID";
        case PatientId: return "患者ID";
        case PatientName: return "患者姓名";
        case CreatedDate: return "创建日期";
        case Diagnosis: return "诊断";
        case Symptoms: return "症状";
        case Treatment: return "治疗方案";
        case DoctorName: return "医生";
        case Notes: return "备注";
        default: return QVariant();
        }
    }
    
    return QAbstractTableModel::headerData(section, orientation, role);
}

int MedicalRecordModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

bool MedicalRecordModel::insertMedicalRecord(const MedicalRecord &record)
{
    if (!validateMedicalRecord(record)) {
        return false;
    }

    // Use DatabaseManager for insertion to maintain consistency
    DatabaseManager &dbManager = DatabaseManager::instance();
    
    // DatabaseManager will emit signals that trigger UI refresh
    // No need to call refreshData() here
    return dbManager.addMedicalRecord(record);
}

bool MedicalRecordModel::updateMedicalRecord(int id, const MedicalRecord &record)
{
    if (id <= 0 || !validateMedicalRecord(record)) {
        return false;
    }

    // Create a copy with the correct ID
    MedicalRecord updateRecord = record;
    updateRecord.id = id;

    // Use DatabaseManager for update to maintain consistency
    DatabaseManager &dbManager = DatabaseManager::instance();
    
    // DatabaseManager will emit signals that trigger UI refresh
    // No need to call refreshData() here
    return dbManager.updateMedicalRecord(updateRecord);
}

bool MedicalRecordModel::deleteMedicalRecord(int id)
{
    if (id <= 0) {
        return false;
    }

    // Use DatabaseManager for deletion to maintain consistency
    DatabaseManager &dbManager = DatabaseManager::instance();
    
    // DatabaseManager will emit signals that trigger UI refresh
    // No need to call refreshData() here
    return dbManager.deleteMedicalRecord(id);
}

void MedicalRecordModel::refreshData()
{
    // 重置分页参数
    m_currentPage = 0;
    m_hasMore = true;
    m_totalCount = m_dbManager->getTotalMedicalRecords();

    beginResetModel();
    m_records.clear();
    endResetModel();

    if (!m_lazyLoadingEnabled) {
        // 懒加载关闭时，一次性加载所有数据
        QList<MedicalRecord> allRecords = m_dbManager->getMedicalRecordsByPage(0, m_totalCount);
        
        beginInsertRows(QModelIndex(), 0, allRecords.size() - 1);
        m_records = allRecords;
        endInsertRows();
        
        m_hasMore = false;
    } else {
        // 懒加载开启时，只加载第一页
        fetchMore(QModelIndex());
    }
}



void MedicalRecordModel::refresh()
{
    refreshData();
}

MedicalRecord MedicalRecordModel::getMedicalRecord(int row) const
{
    if (row < 0 || row >= m_records.size()) {
        return MedicalRecord();
    }
    return m_records.at(row);
}

bool MedicalRecordModel::validateMedicalRecord(const MedicalRecord &record) const
{
    // Check required fields
    if (record.patientId <= 0) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "请选择有效的患者");
        }
        return false;
    }
    
    if (record.diagnosis.trimmed().isEmpty()) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "诊断信息不能为空");
        }
        return false;
    }
    
    if (record.doctorName.trimmed().isEmpty()) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "医生姓名不能为空");
        }
        return false;
    }
    
    // Check field lengths
    if (record.diagnosis.length() > 500) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "诊断信息不能超过500个字符");
        }
        return false;
    }
    
    if (record.symptoms.length() > 1000) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "症状描述不能超过1000个字符");
        }
        return false;
    }
    
    if (record.treatment.length() > 1000) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "治疗方案不能超过1000个字符");
        }
        return false;
    }
    
    if (record.doctorName.length() > 100) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "医生姓名不能超过100个字符");
        }
        return false;
    }
    
    if (record.notes.length() > 2000) {
        if (QApplication::instance()) {
            QMessageBox::warning(nullptr, "验证错误", "备注不能超过2000个字符");
        }
        return false;
    }
    
    return true;
}