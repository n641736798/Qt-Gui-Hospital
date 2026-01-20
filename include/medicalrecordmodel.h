#ifndef MEDICALRECORDMODEL_H
#define MEDICALRECORDMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include "databasemanager.h"

class MedicalRecordModel : public QSqlTableModel
{
    Q_OBJECT

public:
    enum Column {
        Id = 0,
        PatientId,
        PatientName,
        CreatedDate,
        Diagnosis,
        Symptoms,
        Treatment,
        DoctorName,
        Notes,
        ColumnCount
    };

    explicit MedicalRecordModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    
    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    
    // Custom methods for medical record management
    bool insertMedicalRecord(const MedicalRecord &record);
    bool updateMedicalRecord(int id, const MedicalRecord &record);
    bool deleteMedicalRecord(int id);
    MedicalRecord getMedicalRecord(int row) const;
    
    // Data refresh and filtering
    void refreshData();
    void setFilter(const QString &filter);

public slots:
    void refresh();

private:
    void setupModel();
    QString getPatientNameById(int patientId) const;
    bool validateMedicalRecord(const MedicalRecord &record) const;
    
    QSqlDatabase m_database;
    mutable QMap<int, QString> m_patientNameCache;
};

#endif // MEDICALRECORDMODEL_H