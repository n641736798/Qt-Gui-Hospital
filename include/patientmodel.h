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

    // 懒加载分页大小
    static const int PAGE_SIZE = 20;

    explicit PatientModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 懒加载必须实现的接口
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    // 从数据库刷新数据；如果 searchTerm 为空则加载全部，否则按现有 searchPatients 规则搜索
    Q_INVOKABLE void refresh(const QString &searchTerm = QString());

    // 业务辅助接口，便于外部根据行号拿到 Patient 结构体
    Patient getPatient(int row) const;

    // 懒加载配置接口
    void setLazyLoadingEnabled(bool enabled);
    bool isLazyLoadingEnabled() const;

private:
    QList<Patient> m_patients;
    DatabaseManager *m_dbManager;
    QString m_currentSearchTerm; // 当前搜索关键词
    int m_currentPage; // 当前已经加载到的页码
    bool m_hasMore; // 是否还有更多数据可以加载
    int m_totalCount; // 总数据条数
    bool m_lazyLoadingEnabled = true; // 是否启用懒加载，默认开启
};

#endif // PATIENTMODEL_H

