#ifndef MEDICALRECORDMODEL_H
#define MEDICALRECORDMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include "databasemanager.h"

class MedicalRecordModel : public QAbstractTableModel
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

    // 懒加载分页大小
    static const int PAGE_SIZE = 20;

    explicit MedicalRecordModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // 懒加载必须实现的接口
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    
    // Custom methods for medical record management
    bool insertMedicalRecord(const MedicalRecord &record);
    bool updateMedicalRecord(int id, const MedicalRecord &record);
    bool deleteMedicalRecord(int id);
    MedicalRecord getMedicalRecord(int row) const;
    
    // Data refresh
    void refreshData();

public slots:
    void refresh();

private:
    void setupModel();
    bool validateMedicalRecord(const MedicalRecord &record) const;
    
    QSqlDatabase m_database;
    QList<MedicalRecord> m_records; // 内存中的病历数据
    DatabaseManager *m_dbManager;
    int m_currentPage; // 当前已经加载到的页码
    bool m_hasMore; // 是否还有更多数据可以加载
    int m_totalCount; // 总数据条数
};

#endif // MEDICALRECORDMODEL_H