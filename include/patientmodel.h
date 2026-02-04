#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "databasemanager.h"

// MVC 版本中的患者数据模型：
// - 持有一份内存中的 Patient 列表
// - 负责把 DatabaseManager 中的数据转换为 QAbstractTableModel 所需的行列/数据接口
// - 通过 refresh()/search() 等方法从数据库重新拉取数据
class PatientModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        Id = 0,
        Name,
        Phone,
        Email,
        BirthDate,
        Gender,
        MedicalHistory,
        ColumnCount
    };

    explicit PatientModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 从数据库刷新数据；如果 searchTerm 为空则加载全部，否则按现有 searchPatients 规则搜索
    Q_INVOKABLE void refresh(const QString &searchTerm = QString());

    // 业务辅助接口，便于外部根据行号拿到 Patient 结构体
    Patient getPatient(int row) const;

private:
    QList<Patient> m_patients;
    DatabaseManager *m_dbManager;
};

#endif // PATIENTMODEL_H

