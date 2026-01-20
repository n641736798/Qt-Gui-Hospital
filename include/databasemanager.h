#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMessageBox>

struct Patient {
    int id;
    QString name;
    QString phone;
    QString email;
    QString address;
    QString birthDate;
    QString gender;
    QString medicalHistory;
};

struct MedicalRecord {
    int id;
    int patientId;
    QString patientName;
    QDateTime createdDate;
    QString diagnosis;
    QString symptoms;
    QString treatment;
    QString doctorName;
    QString notes;
    
    MedicalRecord() : id(-1), patientId(-1) {}
    
    bool isValid() const {
        return patientId > 0 && !diagnosis.isEmpty() && !doctorName.isEmpty();
    }
};

class DatabaseManager : public QObject
{
    Q_OBJECT

signals:
    // Signal emitted when patient data changes (update or delete)
    void patientDataChanged();
    // Signal emitted when a patient is deleted
    void patientDeleted(int patientId);
    
    // Signals for medical record changes
    void medicalRecordAdded(int recordId);
    void medicalRecordUpdated(int recordId);
    void medicalRecordDeleted(int recordId);
    void medicalRecordDataChanged();

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // Database connection management
    bool connectToDatabase(const QString &host, const QString &database, 
                          const QString &username, const QString &password, int port = 3306);
    void disconnectFromDatabase();
    bool isConnected() const;
    bool testConnection();
    
    // Singleton pattern for global access
    static DatabaseManager& instance();

    // Patient management
    bool addPatient(const Patient &patient);
    bool updatePatient(const Patient &patient);
    bool deletePatient(int patientId);
    QList<Patient> getAllPatients();
    QList<Patient> searchPatients(const QString &searchTerm);
    Patient getPatientById(int id);

    // Medical record management
    bool addMedicalRecord(const MedicalRecord &record);
    bool updateMedicalRecord(const MedicalRecord &record);
    bool deleteMedicalRecord(int recordId);
    QList<MedicalRecord> getAllMedicalRecords();
    QList<MedicalRecord> getMedicalRecordsByPatient(int patientId);
    QList<MedicalRecord> searchMedicalRecords(const QString &searchTerm);
    MedicalRecord getMedicalRecordById(int id);

    // Statistics for dashboard
    int getTotalPatients();
    int getTotalMedicalRecords();
    QMap<QString, int> getPatientsByMonth();
    QMap<QString, int> getPatientsByGender();

    // Error handling
    QString getLastError() const;

private:
    QSqlDatabase db;
    QString m_lastError;
    static DatabaseManager* s_instance;
    
    bool createTables();
    bool createMedicalRecordTable();
    void insertSampleData();
    void handleSqlError(const QSqlError &error, const QString &operation);
    bool executeQuery(QSqlQuery &query, const QString &operation);
};

#endif // DATABASEMANAGER_H