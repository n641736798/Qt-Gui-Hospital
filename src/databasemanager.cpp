#include "databasemanager.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QApplication>
#include <QThread>

// Static instance for singleton pattern
DatabaseManager* DatabaseManager::s_instance = nullptr;

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

DatabaseManager::~DatabaseManager()
{
    disconnectFromDatabase();
}

DatabaseManager& DatabaseManager::instance()
{
    if (!s_instance) {
        s_instance = new DatabaseManager();
    }
    return *s_instance;
}

bool DatabaseManager::connectToDatabase(const QString &host, const QString &database, 
                                      const QString &username, const QString &password, int port)
{
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);

    // Attempt connection with retry logic
    int maxRetries = 3;
    int retryCount = 0;
    bool connected = false;
    
    while (retryCount < maxRetries && !connected) {
        if (db.open()) {
            connected = true;
            qDebug() << "Database connected successfully!";
        } else {
            retryCount++;
            QSqlError error = db.lastError();
            QString nativeCode = error.nativeErrorCode();
            
            // Handle specific MySQL error codes
            if (nativeCode == "2003") {
                // Can't connect to MySQL server
                m_lastError = "无法连接到MySQL服务器。请检查:\n"
                            "1. MySQL服务是否正在运行\n"
                            "2. 主机地址和端口是否正确\n"
                            "3. 防火墙设置是否允许连接";
            } else if (nativeCode == "1045") {
                // Access denied
                m_lastError = "数据库认证失败。请检查:\n"
                            "1. 用户名是否正确\n"
                            "2. 密码是否正确\n"
                            "3. 用户是否有访问权限";
                // Don't retry for authentication errors
                break;
            } else if (nativeCode == "1049") {
                // Unknown database
                m_lastError = QString("数据库 '%1' 不存在。\n请联系系统管理员创建数据库。").arg(database);
                // Don't retry for missing database
                break;
            } else if (nativeCode == "2002") {
                // Connection timeout
                m_lastError = "连接超时。请检查:\n"
                            "1. 网络连接是否正常\n"
                            "2. MySQL服务器是否响应\n"
                            "3. 主机地址是否正确";
            } else {
                m_lastError = QString("数据库连接失败: %1").arg(error.text());
            }
            
            if (retryCount < maxRetries) {
                // Show retry dialog
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("数据库连接错误");
                msgBox.setText(m_lastError);
                msgBox.setInformativeText(QString("连接尝试 %1/%2 失败").arg(retryCount).arg(maxRetries));
                
                msgBox.addButton("重试", QMessageBox::AcceptRole);
                QAbstractButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);
                
                msgBox.exec();
                
                if (msgBox.clickedButton() == cancelButton) {
                    qCritical() << "User cancelled database connection after" << retryCount << "attempts";
                    return false;
                }
                
                // Wait a bit before retrying
                QThread::msleep(1000);
            }
        }
    }
    
    if (!connected) {
        handleSqlError(db.lastError(), "数据库连接");
        return false;
    }

    // Create tables if they don't exist
    if (!createTables()) {
        m_lastError = "创建数据库表失败";
        qDebug() << "Failed to create tables";
        QMessageBox::critical(nullptr, "数据库初始化错误", 
                            "无法创建数据库表。请检查数据库权限。");
        return false;
    }

    return true;
}

bool DatabaseManager::testConnection()
{
    if (!db.isOpen()) {
        m_lastError = "数据库连接未建立";
        return false;
    }
    
    QSqlQuery query("SELECT 1");
    if (!query.exec()) {
        m_lastError = QString("数据库连接测试失败: %1").arg(query.lastError().text());
        return false;
    }
    
    return true;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}

void DatabaseManager::handleSqlError(const QSqlError &error, const QString &operation)
{
    QString errorMessage = QString("%1失败: %2").arg(operation, error.text());
    qCritical() << errorMessage;
    
    // Show user-friendly error message
    if (QApplication::instance()) {
        QMessageBox::critical(nullptr, "数据库错误", errorMessage);
    }
}

bool DatabaseManager::executeQuery(QSqlQuery &query, const QString &operation)
{
    if (!query.exec()) {
        handleSqlError(query.lastError(), operation);
        return false;
    }
    return true;
}

void DatabaseManager::disconnectFromDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::isConnected() const
{
    return db.isOpen();
}

bool DatabaseManager::createTables()
{
    // Start transaction for table creation
    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    QSqlQuery query;
    
    // Create patients table
    QString createPatientsTable = R"(
        CREATE TABLE IF NOT EXISTS patients (
            id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            phone VARCHAR(20),
            email VARCHAR(100),
            address TEXT,
            birth_date DATE,
            gender ENUM('Male', 'Female', 'Other') DEFAULT 'Male',
            medical_history TEXT,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            INDEX idx_name (name),
            INDEX idx_phone (phone)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";

    if (!query.exec(createPatientsTable)) {
        handleSqlError(query.lastError(), "创建患者表");
        db.rollback();
        return false;
    }

    // Create medical records table
    if (!createMedicalRecordTable()) {
        db.rollback();
        return false;
    }

    // Commit transaction
    if (!db.commit()) {
        m_lastError = "提交数据库事务失败";
        db.rollback();
        return false;
    }

    // Insert sample data if tables are empty
    query.exec("SELECT COUNT(*) FROM patients");
    if (query.next() && query.value(0).toInt() == 0) {
        insertSampleData();
    }

    return true;
}

bool DatabaseManager::createMedicalRecordTable()
{
    QSqlQuery query;
    
    QString createMedicalRecordsTable = R"(
        CREATE TABLE IF NOT EXISTS medical_records (
            id INT AUTO_INCREMENT PRIMARY KEY,
            patient_id INT NOT NULL,
            created_date DATETIME DEFAULT CURRENT_TIMESTAMP,
            diagnosis TEXT NOT NULL,
            symptoms TEXT,
            treatment TEXT,
            doctor_name VARCHAR(100) NOT NULL,
            notes TEXT,
            INDEX idx_patient_id (patient_id),
            INDEX idx_created_date (created_date),
            INDEX idx_doctor_name (doctor_name),
            INDEX idx_patient_date (patient_id, created_date),
            FOREIGN KEY (patient_id) REFERENCES patients(id) ON DELETE CASCADE
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";

    if (!query.exec(createMedicalRecordsTable)) {
        handleSqlError(query.lastError(), "创建病历表");
        return false;
    }

    qDebug() << "Medical records table created successfully";
    return true;
}

void DatabaseManager::insertSampleData()
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO patients (name, phone, email, address, birth_date, gender, medical_history) 
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

    // Sample patients
    QList<Patient> samplePatients;
    
    Patient p1 = {0, "John Smith", "555-0101", "john.smith@email.com", "123 Main St", "1985-03-15", "Male", "Hypertension"};
    Patient p2 = {0, "Sarah Johnson", "555-0102", "sarah.j@email.com", "456 Oak Ave", "1990-07-22", "Female", "Diabetes Type 2"};
    Patient p3 = {0, "Michael Brown", "555-0103", "m.brown@email.com", "789 Pine Rd", "1978-11-08", "Male", "Asthma"};
    Patient p4 = {0, "Emily Davis", "555-0104", "emily.davis@email.com", "321 Elm St", "1995-01-30", "Female", "No known allergies"};
    Patient p5 = {0, "Robert Wilson", "555-0105", "r.wilson@email.com", "654 Maple Dr", "1982-09-12", "Male", "Heart disease"};
    
    samplePatients << p1 << p2 << p3 << p4 << p5;

    for (const auto &patient : samplePatients) {
        query.addBindValue(patient.name);
        query.addBindValue(patient.phone);
        query.addBindValue(patient.email);
        query.addBindValue(patient.address);
        query.addBindValue(patient.birthDate);
        query.addBindValue(patient.gender);
        query.addBindValue(patient.medicalHistory);
        
        if (!query.exec()) {
            qDebug() << "Failed to insert sample patient:" << query.lastError().text();
        }
    }
}

bool DatabaseManager::addPatient(const Patient &patient)
{
    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO patients (name, phone, email, address, birth_date, gender, medical_history) 
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");
    
    query.addBindValue(patient.name);
    query.addBindValue(patient.phone);
    query.addBindValue(patient.email);
    query.addBindValue(patient.address);
    query.addBindValue(patient.birthDate);
    query.addBindValue(patient.gender);
    query.addBindValue(patient.medicalHistory);

    if (!executeQuery(query, "添加患者")) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::updatePatient(const Patient &patient)
{
    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        UPDATE patients 
        SET name=?, phone=?, email=?, address=?, birth_date=?, gender=?, medical_history=? 
        WHERE id=?
    )");
    
    query.addBindValue(patient.name);
    query.addBindValue(patient.phone);
    query.addBindValue(patient.email);
    query.addBindValue(patient.address);
    query.addBindValue(patient.birthDate);
    query.addBindValue(patient.gender);
    query.addBindValue(patient.medicalHistory);
    query.addBindValue(patient.id);

    if (!executeQuery(query, "更新患者信息")) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        return false;
    }

    // Emit signal to notify that patient data has changed
    emit patientDataChanged();

    return true;
}

bool DatabaseManager::deletePatient(int patientId)
{
    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    // Note: Medical records associated with this patient will be automatically
    // deleted due to the ON DELETE CASCADE foreign key constraint
    QSqlQuery query;
    query.prepare("DELETE FROM patients WHERE id = ?");
    query.addBindValue(patientId);

    if (!executeQuery(query, "删除患者")) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        return false;
    }

    // Emit signals to notify that patient has been deleted
    // This will trigger UI refresh in medical record widget
    emit patientDeleted(patientId);
    emit patientDataChanged();

    return true;
}

QList<Patient> DatabaseManager::getAllPatients()
{
    QList<Patient> patients;
    QSqlQuery query("SELECT id, name, phone, email, address, birth_date, gender, medical_history FROM patients ORDER BY name");

    while (query.next()) {
        Patient patient;
        patient.id = query.value(0).toInt();
        patient.name = query.value(1).toString();
        patient.phone = query.value(2).toString();
        patient.email = query.value(3).toString();
        patient.address = query.value(4).toString();
        patient.birthDate = query.value(5).toString();
        patient.gender = query.value(6).toString();
        patient.medicalHistory = query.value(7).toString();
        patients.append(patient);
    }

    return patients;
}

QList<Patient> DatabaseManager::searchPatients(const QString &searchTerm)
{
    QList<Patient> patients;
    QSqlQuery query;
    query.prepare(R"(
        SELECT id, name, phone, email, address, birth_date, gender, medical_history 
        FROM patients 
        WHERE name LIKE ? OR phone LIKE ? OR email LIKE ? 
        ORDER BY name
    )");
    
    QString searchPattern = "%" + searchTerm + "%";
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);

    if (query.exec()) {
        while (query.next()) {
            Patient patient;
            patient.id = query.value(0).toInt();
            patient.name = query.value(1).toString();
            patient.phone = query.value(2).toString();
            patient.email = query.value(3).toString();
            patient.address = query.value(4).toString();
            patient.birthDate = query.value(5).toString();
            patient.gender = query.value(6).toString();
            patient.medicalHistory = query.value(7).toString();
            patients.append(patient);
        }
    }

    return patients;
}

Patient DatabaseManager::getPatientById(int id)
{
    Patient patient;
    QSqlQuery query;
    query.prepare("SELECT id, name, phone, email, address, birth_date, gender, medical_history FROM patients WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        patient.id = query.value(0).toInt();
        patient.name = query.value(1).toString();
        patient.phone = query.value(2).toString();
        patient.email = query.value(3).toString();
        patient.address = query.value(4).toString();
        patient.birthDate = query.value(5).toString();
        patient.gender = query.value(6).toString();
        patient.medicalHistory = query.value(7).toString();
    }

    return patient;
}

int DatabaseManager::getTotalPatients()
{
    QSqlQuery query("SELECT COUNT(*) FROM patients");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int DatabaseManager::getTotalMedicalRecords()
{
    QSqlQuery query("SELECT COUNT(*) FROM medical_records");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Medical Record Management Functions
bool DatabaseManager::addMedicalRecord(const MedicalRecord &record)
{
    if (!record.isValid()) {
        m_lastError = "病历数据无效：患者ID、诊断信息和医生姓名不能为空";
        QMessageBox::warning(nullptr, "数据验证错误", m_lastError);
        return false;
    }

    // First verify patient exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM patients WHERE id = ?");
    checkQuery.addBindValue(record.patientId);
    
    if (!checkQuery.exec()) {
        m_lastError = QString("验证患者信息时发生错误: %1").arg(checkQuery.lastError().text());
        QMessageBox::critical(nullptr, "数据库错误", m_lastError);
        return false;
    }
    
    if (!checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        m_lastError = QString("患者ID %1 不存在。\n\n可能的原因:\n"
                            "1. 患者已被删除\n"
                            "2. 患者ID输入错误\n\n"
                            "请选择有效的患者或创建新患者。").arg(record.patientId);
        QMessageBox::warning(nullptr, "外键约束错误", m_lastError);
        return false;
    }

    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        QMessageBox::critical(nullptr, "事务错误", 
                            "无法开始数据库事务。请检查数据库连接。");
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO medical_records (patient_id, diagnosis, symptoms, treatment, doctor_name, notes) 
        VALUES (?, ?, ?, ?, ?, ?)
    )");
    
    query.addBindValue(record.patientId);
    query.addBindValue(record.diagnosis);
    query.addBindValue(record.symptoms);
    query.addBindValue(record.treatment);
    query.addBindValue(record.doctorName);
    query.addBindValue(record.notes);

    if (!query.exec()) {
        db.rollback();
        QSqlError error = query.lastError();
        QString nativeCode = error.nativeErrorCode();
        
        // Handle specific MySQL error codes
        if (nativeCode == "1452") {
            // Foreign key constraint fails
            m_lastError = QString("无法添加病历记录：关联的患者不存在。\n\n"
                                "患者ID: %1\n\n"
                                "请确保患者信息存在后再试。").arg(record.patientId);
            QMessageBox::warning(nullptr, "外键约束错误", m_lastError);
        } else if (nativeCode == "1406") {
            // Data too long for column
            m_lastError = "数据长度超出限制。请检查:\n"
                        "• 诊断信息不超过500字符\n"
                        "• 症状描述不超过1000字符\n"
                        "• 治疗方案不超过1000字符\n"
                        "• 医生姓名不超过100字符";
            QMessageBox::warning(nullptr, "数据验证错误", m_lastError);
        } else {
            m_lastError = QString("添加病历记录失败: %1").arg(error.text());
            handleSqlError(error, "添加病历记录");
        }
        return false;
    }

    // Get the ID of the newly inserted record
    int newRecordId = query.lastInsertId().toInt();

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        QMessageBox::critical(nullptr, "事务错误", 
                            "无法提交数据库事务。更改已回滚。");
        return false;
    }

    // Emit signals to notify that medical record has been added
    emit medicalRecordAdded(newRecordId);
    emit medicalRecordDataChanged();

    return true;
}

bool DatabaseManager::updateMedicalRecord(const MedicalRecord &record)
{
    if (!record.isValid() || record.id <= 0) {
        m_lastError = "病历数据无效";
        return false;
    }

    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        UPDATE medical_records 
        SET patient_id=?, diagnosis=?, symptoms=?, treatment=?, doctor_name=?, notes=? 
        WHERE id=?
    )");
    
    query.addBindValue(record.patientId);
    query.addBindValue(record.diagnosis);
    query.addBindValue(record.symptoms);
    query.addBindValue(record.treatment);
    query.addBindValue(record.doctorName);
    query.addBindValue(record.notes);
    query.addBindValue(record.id);

    if (!executeQuery(query, "更新病历记录")) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        return false;
    }

    // Emit signals to notify that medical record has been updated
    emit medicalRecordUpdated(record.id);
    emit medicalRecordDataChanged();

    return true;
}

bool DatabaseManager::deleteMedicalRecord(int recordId)
{
    if (!db.transaction()) {
        m_lastError = "无法开始数据库事务";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM medical_records WHERE id = ?");
    query.addBindValue(recordId);

    if (!executeQuery(query, "删除病历记录")) {
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = "提交事务失败";
        db.rollback();
        return false;
    }

    // Emit signals to notify that medical record has been deleted
    emit medicalRecordDeleted(recordId);
    emit medicalRecordDataChanged();

    return true;
}

QList<MedicalRecord> DatabaseManager::getAllMedicalRecords()
{
    QList<MedicalRecord> records;
    QSqlQuery query(R"(
        SELECT mr.id, mr.patient_id, p.name, mr.created_date, mr.diagnosis, 
               mr.symptoms, mr.treatment, mr.doctor_name, mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        ORDER BY mr.created_date DESC
    )");

    while (query.next()) {
        MedicalRecord record;
        record.id = query.value(0).toInt();
        record.patientId = query.value(1).toInt();
        record.patientName = query.value(2).toString();
        if (record.patientName.isEmpty()) {
            record.patientName = "患者已删除";
        }
        record.createdDate = query.value(3).toDateTime();
        record.diagnosis = query.value(4).toString();
        record.symptoms = query.value(5).toString();
        record.treatment = query.value(6).toString();
        record.doctorName = query.value(7).toString();
        record.notes = query.value(8).toString();
        records.append(record);
    }

    return records;
}

QList<MedicalRecord> DatabaseManager::getMedicalRecordsByPatient(int patientId)
{
    QList<MedicalRecord> records;
    QSqlQuery query;
    query.prepare(R"(
        SELECT mr.id, mr.patient_id, p.name, mr.created_date, mr.diagnosis, 
               mr.symptoms, mr.treatment, mr.doctor_name, mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        WHERE mr.patient_id = ?
        ORDER BY mr.created_date DESC
    )");
    query.addBindValue(patientId);

    if (query.exec()) {
        while (query.next()) {
            MedicalRecord record;
            record.id = query.value(0).toInt();
            record.patientId = query.value(1).toInt();
            record.patientName = query.value(2).toString();
            if (record.patientName.isEmpty()) {
                record.patientName = "患者已删除";
            }
            record.createdDate = query.value(3).toDateTime();
            record.diagnosis = query.value(4).toString();
            record.symptoms = query.value(5).toString();
            record.treatment = query.value(6).toString();
            record.doctorName = query.value(7).toString();
            record.notes = query.value(8).toString();
            records.append(record);
        }
    }

    return records;
}

QList<MedicalRecord> DatabaseManager::searchMedicalRecords(const QString &searchTerm)
{
    QList<MedicalRecord> records;
    QSqlQuery query;
    query.prepare(R"(
        SELECT mr.id, mr.patient_id, p.name, mr.created_date, mr.diagnosis, 
               mr.symptoms, mr.treatment, mr.doctor_name, mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        WHERE p.name LIKE ? OR mr.diagnosis LIKE ? OR mr.doctor_name LIKE ?
        ORDER BY mr.created_date DESC
    )");
    
    QString searchPattern = "%" + searchTerm + "%";
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);
    query.addBindValue(searchPattern);

    if (query.exec()) {
        while (query.next()) {
            MedicalRecord record;
            record.id = query.value(0).toInt();
            record.patientId = query.value(1).toInt();
            record.patientName = query.value(2).toString();
            if (record.patientName.isEmpty()) {
                record.patientName = "患者已删除";
            }
            record.createdDate = query.value(3).toDateTime();
            record.diagnosis = query.value(4).toString();
            record.symptoms = query.value(5).toString();
            record.treatment = query.value(6).toString();
            record.doctorName = query.value(7).toString();
            record.notes = query.value(8).toString();
            records.append(record);
        }
    }

    return records;
}

MedicalRecord DatabaseManager::getMedicalRecordById(int id)
{
    MedicalRecord record;
    QSqlQuery query;
    query.prepare(R"(
        SELECT mr.id, mr.patient_id, p.name, mr.created_date, mr.diagnosis, 
               mr.symptoms, mr.treatment, mr.doctor_name, mr.notes
        FROM medical_records mr
        LEFT JOIN patients p ON mr.patient_id = p.id
        WHERE mr.id = ?
    )");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        record.id = query.value(0).toInt();
        record.patientId = query.value(1).toInt();
        record.patientName = query.value(2).toString();
        if (record.patientName.isEmpty()) {
            record.patientName = "患者已删除";
        }
        record.createdDate = query.value(3).toDateTime();
        record.diagnosis = query.value(4).toString();
        record.symptoms = query.value(5).toString();
        record.treatment = query.value(6).toString();
        record.doctorName = query.value(7).toString();
        record.notes = query.value(8).toString();
    }

    return record;
}

QMap<QString, int> DatabaseManager::getPatientsByMonth()
{
    QMap<QString, int> monthlyData;
    QSqlQuery query(R"(
        SELECT DATE_FORMAT(created_at, '%Y-%m') as month, COUNT(*) as count 
        FROM patients 
        WHERE created_at >= DATE_SUB(NOW(), INTERVAL 6 MONTH)
        GROUP BY month 
        ORDER BY month
    )");

    while (query.next()) {
        monthlyData[query.value(0).toString()] = query.value(1).toInt();
    }

    return monthlyData;
}

QMap<QString, int> DatabaseManager::getPatientsByGender()
{
    QMap<QString, int> genderData;
    QSqlQuery query("SELECT gender, COUNT(*) FROM patients GROUP BY gender");

    while (query.next()) {
        genderData[query.value(0).toString()] = query.value(1).toInt();
    }

    return genderData;
}