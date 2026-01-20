#include "medicalrecordmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>

MedicalRecordModel::MedicalRecordModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db), m_database(db)
{
    if (!m_database.isValid()) {
        m_database = QSqlDatabase::database();
    }
    
    setupModel();
}

void MedicalRecordModel::setupModel()
{
    // Set the table name
    setTable("medical_records");
    
    // Set edit strategy to manual submit for better control
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // Set up the query to join with patients table for patient names
    QString queryStr = R"(
        SELECT 
            mr.id,
            mr.patient_id,
            COALESCE(p.name, '患者已删除') as patient_name,
            mr.created_date,
            mr.diagnosis,
            mr.symptoms,
            mr.treatment,
            mr.doctor_name,
            mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        ORDER BY mr.created_date DESC
    )";
    
    setQuery(queryStr, m_database);
    
    // Set headers
    setHeaderData(Id, Qt::Horizontal, "病历ID");
    setHeaderData(PatientId, Qt::Horizontal, "患者ID");
    setHeaderData(PatientName, Qt::Horizontal, "患者姓名");
    setHeaderData(CreatedDate, Qt::Horizontal, "创建日期");
    setHeaderData(Diagnosis, Qt::Horizontal, "诊断");
    setHeaderData(Symptoms, Qt::Horizontal, "症状");
    setHeaderData(Treatment, Qt::Horizontal, "治疗方案");
    setHeaderData(DoctorName, Qt::Horizontal, "医生");
    setHeaderData(Notes, Qt::Horizontal, "备注");
}

QVariant MedicalRecordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        // Handle special formatting for certain columns
        switch (index.column()) {
        case CreatedDate: {
            QDateTime dateTime = QSqlTableModel::data(index, role).toDateTime();
            return dateTime.toString("yyyy-MM-dd hh:mm");
        }
        case PatientName: {
            // Patient name is already handled by the JOIN query
            return QSqlTableModel::data(index, role);
        }
        case Diagnosis:
        case Symptoms:
        case Treatment:
        case Notes: {
            // Truncate long text for display
            QString text = QSqlTableModel::data(index, role).toString();
            if (text.length() > 50) {
                return text.left(47) + "...";
            }
            return text;
        }
        default:
            return QSqlTableModel::data(index, role);
        }
    }
    
    return QSqlTableModel::data(index, role);
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
    
    return QSqlTableModel::headerData(section, orientation, role);
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

MedicalRecord MedicalRecordModel::getMedicalRecord(int row) const
{
    MedicalRecord record;
    
    if (row < 0 || row >= rowCount()) {
        return record;
    }

    QSqlRecord sqlRecord = this->record(row);
    
    record.id = sqlRecord.value("id").toInt();
    record.patientId = sqlRecord.value("patient_id").toInt();
    record.patientName = sqlRecord.value("patient_name").toString();
    record.createdDate = sqlRecord.value("created_date").toDateTime();
    record.diagnosis = sqlRecord.value("diagnosis").toString();
    record.symptoms = sqlRecord.value("symptoms").toString();
    record.treatment = sqlRecord.value("treatment").toString();
    record.doctorName = sqlRecord.value("doctor_name").toString();
    record.notes = sqlRecord.value("notes").toString();
    
    return record;
}

void MedicalRecordModel::refreshData()
{
    // Clear the patient name cache
    m_patientNameCache.clear();
    
    // Refresh the model data
    QString queryStr = R"(
        SELECT 
            mr.id,
            mr.patient_id,
            COALESCE(p.name, '患者已删除') as patient_name,
            mr.created_date,
            mr.diagnosis,
            mr.symptoms,
            mr.treatment,
            mr.doctor_name,
            mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        ORDER BY mr.created_date DESC
    )";
    
    setQuery(queryStr, m_database);
}

void MedicalRecordModel::setFilter(const QString &filter)
{
    QString queryStr;
    
    if (filter.isEmpty()) {
        queryStr = R"(
            SELECT 
                mr.id,
                mr.patient_id,
                COALESCE(p.name, '患者已删除') as patient_name,
                mr.created_date,
                mr.diagnosis,
                mr.symptoms,
                mr.treatment,
                mr.doctor_name,
                mr.notes
            FROM medical_records mr
            LEFT JOIN patients p ON mr.patient_id = p.id
            ORDER BY mr.created_date DESC
        )";
    } else {
        queryStr = QString(R"(
            SELECT 
                mr.id,
                mr.patient_id,
                COALESCE(p.name, '患者已删除') as patient_name,
                mr.created_date,
                mr.diagnosis,
                mr.symptoms,
                mr.treatment,
                mr.doctor_name,
                mr.notes
            FROM medical_records mr
            LEFT JOIN patients p ON mr.patient_id = p.id
            WHERE p.name LIKE '%%1%' 
               OR mr.diagnosis LIKE '%%1%' 
               OR mr.doctor_name LIKE '%%1%'
            ORDER BY mr.created_date DESC
        )").arg(filter);
    }
    
    setQuery(queryStr, m_database);
}

void MedicalRecordModel::refresh()
{
    refreshData();
}

QString MedicalRecordModel::getPatientNameById(int patientId) const
{
    // Check cache first
    if (m_patientNameCache.contains(patientId)) {
        return m_patientNameCache[patientId];
    }
    
    // Query database
    QSqlQuery query(m_database);
    query.prepare("SELECT name FROM patients WHERE id = ?");
    query.addBindValue(patientId);
    
    QString patientName = "患者已删除";
    if (query.exec() && query.next()) {
        patientName = query.value(0).toString();
    }
    
    // Cache the result
    m_patientNameCache[patientId] = patientName;
    
    return patientName;
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