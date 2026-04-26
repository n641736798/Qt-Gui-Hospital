#ifndef PATIENTWIDGET_H
#define PATIENTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "patientmodel.h"
#include "patientdialog.h"

// 使用 MVC（QTableView + PatientModel）的患者管理界面。
// 用于与 MainWindow 中的非 MVC（QTableWidget）版本进行对比学习。
class PatientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientWidget(QWidget *parent = nullptr);

public slots:
    void refreshData();

private slots:
    void onSearchTextChanged(const QString &text);
    void onAddPatient();
    void onEditPatient();
    void onDeletePatient();
    void onTableDoubleClicked(const QModelIndex &index);
    void onSelectionChanged();

private:
    void setupUI();
    void setupConnections();
    void updateButtonStates();

    PatientModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxyModel = nullptr;
    QTableView *m_tableView = nullptr;
    QLineEdit *m_searchEdit = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_editButton = nullptr;
    QPushButton *m_deleteButton = nullptr;
};

#endif // PATIENTWIDGET_H

