#ifndef MEDICALRECORDWIDGET_H
#define MEDICALRECORDWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QModelIndex>
#include <QItemSelectionModel>
#include "medicalrecordmodel.h"
#include "medicalrecorddialog.h"
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MedicalRecordWidget; }
QT_END_NAMESPACE

class MedicalRecordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalRecordWidget(QWidget *parent = nullptr);
    ~MedicalRecordWidget();

public slots:
    void refreshData();
    void onAddRecord();
    void onEditRecord();
    void onDeleteRecord();
    void onSearchChanged(const QString &text);
    void onSelectionChanged();
    void onMedicalRecordDataChanged();
    void onPatientDataChanged();

private slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    void setupUI();
    void setupConnections();
    void updateButtonStates();
    void showMessage(const QString &message);
    void hideMessage();
    
    Ui::MedicalRecordWidget *ui;
    MedicalRecordModel *m_model;
};

#endif // MEDICALRECORDWIDGET_H