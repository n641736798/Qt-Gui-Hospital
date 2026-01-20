#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "databasemanager.h"

class PatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientDialog(QWidget *parent = nullptr);
    explicit PatientDialog(const Patient &patient, QWidget *parent = nullptr);

    Patient getPatient() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    void setupUI();
    void populateFields(const Patient &patient);

    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QLineEdit *addressEdit;
    QDateEdit *birthDateEdit;
    QComboBox *genderCombo;
    QTextEdit *medicalHistoryEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    Patient currentPatient;
    bool isEditMode;
};

#endif // PATIENTDIALOG_H