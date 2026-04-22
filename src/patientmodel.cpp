#include "patientmodel.h"

PatientModel::PatientModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_dbManager(&DatabaseManager::instance())
    , m_currentPage(0)
    , m_hasMore(true)
    , m_totalCount(0)
{
    refresh();
}

int PatientModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_patients.size();
}

int PatientModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return ColumnCount;
}

QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= m_patients.size() ||
        index.column() < 0 || index.column() >= ColumnCount) {
        return {};
    }

    const Patient &patient = m_patients.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case Id:           return patient.id;
        case Name:         return patient.name;
        case Phone:        return patient.phone;
        case Email:        return patient.email;
        case BirthDate:    return patient.birthDate;
        case Gender:       return patient.gender;
        case MedicalHistory: {
            QString history = patient.medicalHistory;
            if (history.length() > 50) {
                history = history.left(47) + "...";
            }
            return history;
        }
        default:
            break;
        }
    }

    return {};
}

QVariant PatientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case Id:             return QStringLiteral("编号");
        case Name:           return QStringLiteral("姓名");
        case Phone:          return QStringLiteral("电话");
        case Email:          return QStringLiteral("邮箱");
        case BirthDate:      return QStringLiteral("出生日期");
        case Gender:         return QStringLiteral("性别");
        case MedicalHistory: return QStringLiteral("病史");
        default:
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool PatientModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid())
        return false;
    return m_hasMore;
}

void PatientModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid() || !m_hasMore)
        return;

    QList<Patient> newPatients;
    if (m_currentSearchTerm.isEmpty()) {
        newPatients = m_dbManager->getPatientsByPage(m_currentPage, PAGE_SIZE);
    } else {
        newPatients = m_dbManager->searchPatientsByPage(m_currentSearchTerm, m_currentPage, PAGE_SIZE);
    }

    if (newPatients.isEmpty()) {
        m_hasMore = false;
        return;
    }

    beginInsertRows(QModelIndex(), m_patients.size(), m_patients.size() + newPatients.size() - 1);
    m_patients.append(newPatients);
    endInsertRows();

    m_currentPage++;
    // 如果返回的数据小于分页大小，说明没有更多了
    if (newPatients.size() < PAGE_SIZE) {
        m_hasMore = false;
    }
}

void PatientModel::refresh(const QString &searchTerm)
{
    if (!m_dbManager || !m_dbManager->isConnected()) {
        beginResetModel();
        m_patients.clear();
        endResetModel();
        return;
    }

    // 重置分页参数
    m_currentSearchTerm = searchTerm.trimmed();
    m_currentPage = 0;
    m_hasMore = true;
    m_totalCount = m_dbManager->getTotalPatients();

    beginResetModel();
    m_patients.clear();
    endResetModel();

    // 加载第一页
    fetchMore(QModelIndex());
}

Patient PatientModel::getPatient(int row) const
{
    if (row < 0 || row >= m_patients.size()) {
        return Patient{};
    }
    return m_patients.at(row);
}

