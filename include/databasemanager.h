#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardItemModel>

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

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectToDatabase(const QString &host, const QString &database, 
                          const QString &username, const QString &password, int port = 3306);
    void disconnectFromDatabase();
    bool isConnected() const;

    // Patient management
    bool addPatient(const Patient &patient);
    bool updatePatient(const Patient &patient);
    bool deletePatient(int patientId);
    QList<Patient> getAllPatients();
    QList<Patient> searchPatients(const QString &searchTerm);
    Patient getPatientById(int id);

    // Statistics for dashboard
    int getTotalPatients();
    QMap<QString, int> getPatientsByMonth();
    QMap<QString, int> getPatientsByGender();

private:
    QSqlDatabase db;
    bool createTables();
    void insertSampleData();
};

#endif // DATABASEMANAGER_H