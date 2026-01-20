#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

DatabaseManager::~DatabaseManager()
{
    disconnectFromDatabase();
}

bool DatabaseManager::connectToDatabase(const QString &host, const QString &database, 
                                      const QString &username, const QString &password, int port)
{
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(port);

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully!";
    
    // Create tables if they don't exist
    if (!createTables()) {
        qDebug() << "Failed to create tables";
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
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createPatientsTable)) {
        qDebug() << "Failed to create patients table:" << query.lastError().text();
        return false;
    }

    // Insert sample data if table is empty
    query.exec("SELECT COUNT(*) FROM patients");
    if (query.next() && query.value(0).toInt() == 0) {
        insertSampleData();
    }

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
    QList<Patient> samplePatients = {
        {0, "John Smith", "555-0101", "john.smith@email.com", "123 Main St", "1985-03-15", "Male", "Hypertension"},
        {0, "Sarah Johnson", "555-0102", "sarah.j@email.com", "456 Oak Ave", "1990-07-22", "Female", "Diabetes Type 2"},
        {0, "Michael Brown", "555-0103", "m.brown@email.com", "789 Pine Rd", "1978-11-08", "Male", "Asthma"},
        {0, "Emily Davis", "555-0104", "emily.davis@email.com", "321 Elm St", "1995-01-30", "Female", "No known allergies"},
        {0, "Robert Wilson", "555-0105", "r.wilson@email.com", "654 Maple Dr", "1982-09-12", "Male", "Heart disease"}
    };

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

    if (!query.exec()) {
        qDebug() << "Failed to add patient:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::updatePatient(const Patient &patient)
{
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

    if (!query.exec()) {
        qDebug() << "Failed to update patient:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::deletePatient(int patientId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM patients WHERE id = ?");
    query.addBindValue(patientId);

    if (!query.exec()) {
        qDebug() << "Failed to delete patient:" << query.lastError().text();
        return false;
    }

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