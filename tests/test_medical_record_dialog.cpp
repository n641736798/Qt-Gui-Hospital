#include <QtTest/QtTest>
#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include "../include/medicalrecorddialog.h"
#include "../include/databasemanager.h"

class TestMedicalRecordDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDialogCreation();
    void testUIComponents();
    void testCreateMode();
    void testEditMode();
    void testValidation();
    void testPatientSelection();

private:
    DatabaseManager *dbManager;
};

void TestMedicalRecordDialog::initTestCase()
{
    // Initialize database connection for testing
    dbManager = &DatabaseManager::instance();
    // Note: In a real test, we would set up a test database
}

void TestMedicalRecordDialog::cleanupTestCase()
{
    // Cleanup test data
}

void TestMedicalRecordDialog::testDialogCreation()
{
    // Test creating dialog in Create mode
    MedicalRecordDialog createDialog(MedicalRecordDialog::Create);
    QVERIFY(createDialog.windowTitle().contains("添加"));
    
    // Test creating dialog in Edit mode
    MedicalRecordDialog editDialog(MedicalRecordDialog::Edit);
    QVERIFY(editDialog.windowTitle().contains("编辑"));
}

void TestMedicalRecordDialog::testUIComponents()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Create);
    
    // Verify essential UI components exist
    QVERIFY(dialog.findChild<QComboBox*>("patientComboBox") != nullptr);
    QVERIFY(dialog.findChild<QLineEdit*>("diagnosisEdit") != nullptr);
    QVERIFY(dialog.findChild<QTextEdit*>("symptomsEdit") != nullptr);
    QVERIFY(dialog.findChild<QTextEdit*>("treatmentEdit") != nullptr);
    QVERIFY(dialog.findChild<QLineEdit*>("doctorNameEdit") != nullptr);
    QVERIFY(dialog.findChild<QTextEdit*>("notesEdit") != nullptr);
    QVERIFY(dialog.findChild<QPushButton*>("saveButton") != nullptr);
    QVERIFY(dialog.findChild<QPushButton*>("cancelButton") != nullptr);
    QVERIFY(dialog.findChild<QLabel*>("errorLabel") != nullptr);
    
    // Verify patient details components
    QVERIFY(dialog.findChild<QLabel*>("patientNameValue") != nullptr);
    QVERIFY(dialog.findChild<QLabel*>("patientAgeValue") != nullptr);
    QVERIFY(dialog.findChild<QLabel*>("patientGenderValue") != nullptr);
    QVERIFY(dialog.findChild<QLabel*>("patientPhoneValue") != nullptr);
}

void TestMedicalRecordDialog::testCreateMode()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Create);
    
    // In create mode, patient combo should be enabled
    QComboBox *patientCombo = dialog.findChild<QComboBox*>("patientComboBox");
    QVERIFY(patientCombo->isEnabled());
    
    // Initially, no patient should be selected
    QCOMPARE(patientCombo->currentData().toInt(), -1);
}

void TestMedicalRecordDialog::testEditMode()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Edit);
    
    // Create a test medical record
    MedicalRecord testRecord;
    testRecord.id = 1;
    testRecord.patientId = 1;
    testRecord.diagnosis = "Test Diagnosis";
    testRecord.symptoms = "Test Symptoms";
    testRecord.treatment = "Test Treatment";
    testRecord.doctorName = "Dr. Test";
    testRecord.notes = "Test Notes";
    
    dialog.setMedicalRecord(testRecord);
    
    // Verify fields are populated
    QLineEdit *diagnosisEdit = dialog.findChild<QLineEdit*>("diagnosisEdit");
    QCOMPARE(diagnosisEdit->text(), testRecord.diagnosis);
    
    QLineEdit *doctorNameEdit = dialog.findChild<QLineEdit*>("doctorNameEdit");
    QCOMPARE(doctorNameEdit->text(), testRecord.doctorName);
    
    QTextEdit *symptomsEdit = dialog.findChild<QTextEdit*>("symptomsEdit");
    QCOMPARE(symptomsEdit->toPlainText(), testRecord.symptoms);
    
    // In edit mode, patient combo should be disabled
    QComboBox *patientCombo = dialog.findChild<QComboBox*>("patientComboBox");
    QVERIFY(!patientCombo->isEnabled());
}

void TestMedicalRecordDialog::testValidation()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Create);
    
    // Test empty required fields validation
    QLineEdit *diagnosisEdit = dialog.findChild<QLineEdit*>("diagnosisEdit");
    QLineEdit *doctorNameEdit = dialog.findChild<QLineEdit*>("doctorNameEdit");
    QLabel *errorLabel = dialog.findChild<QLabel*>("errorLabel");
    
    // Clear required fields
    diagnosisEdit->clear();
    doctorNameEdit->clear();
    
    // Try to save - should show validation errors
    QPushButton *saveButton = dialog.findChild<QPushButton*>("saveButton");
    QTest::mouseClick(saveButton, Qt::LeftButton);
    
    // Error label should be visible
    QVERIFY(errorLabel->isVisible());
    QVERIFY(!errorLabel->text().isEmpty());
}

void TestMedicalRecordDialog::testPatientSelection()
{
    MedicalRecordDialog dialog(MedicalRecordDialog::Create);
    
    QComboBox *patientCombo = dialog.findChild<QComboBox*>("patientComboBox");
    
    // Initially should have "请选择患者..." option
    QVERIFY(patientCombo->count() > 0);
    QVERIFY(patientCombo->itemText(0).contains("请选择患者"));
    
    // Patient details frame should be hidden initially
    QWidget *patientDetailsFrame = dialog.findChild<QWidget*>("patientDetailsFrame");
    QVERIFY(!patientDetailsFrame->isVisible());
}

QTEST_MAIN(TestMedicalRecordDialog)
#include "test_medical_record_dialog.moc"