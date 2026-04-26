#ifndef MEDICALRECORDDIALOG_H
#define MEDICALRECORDDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDateTime>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MedicalRecordDialog; }
QT_END_NAMESPACE

class MedicalRecordDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode { Create, Edit };
    
    explicit MedicalRecordDialog(Mode mode, QWidget *parent = nullptr);
    ~MedicalRecordDialog();
    
    // Set patient for create mode
    void setPatientId(int patientId);
    
    // Set medical record for edit mode
    void setMedicalRecord(const MedicalRecord &record);
    
    // Get the medical record data from the form
    MedicalRecord getMedicalRecord() const;

private slots:
    void onPatientChanged();
    void onSaveClicked();
    void onCancelClicked();
    void onTextChanged();

private:
    void setupUI();
    void setupConnections();
    void setupValidation();
    void loadPatients();
    void loadPatientInfo(int patientId);
    bool validateInput();
    void showError(const QString &message);
    void hideError();
    void resetFieldStyles();
    void highlightErrorField(QWidget *field);
    int calculateAge(const QString &birthDate) const;
    QString mapGenderToDisplay(const QString &gender) const;
    
    Ui::MedicalRecordDialog *ui;
    Mode m_mode;
    MedicalRecord m_record;
    DatabaseManager *m_dbManager;
    bool m_dataLoaded;
};

#endif // MEDICALRECORDDIALOG_H