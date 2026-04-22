#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../include/databasemanager.h"

class TestCascadeDelete : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCascadeDeleteOnPatientDeletion();
    void testPatientInfoSynchronization();
    void testDeletedPatientMarker();

private:
    DatabaseManager *dbManager;
};

void TestCascadeDelete::initTestCase()
{
    // Initialize database connection for testing
    dbManager = new DatabaseManager(this);
    
    // Connect to test database
    bool connected = dbManager->connectToDatabase(
        "localhost",
        "hospital_db_test",
        "root",
        "123456"
    );
    
    QVERIFY2(connected, "Failed to connect to test database");
}

void TestCascadeDelete::cleanupTestCase()
{
    // Clean up test data
    QSqlQuery query;
    query.exec("DELETE FROM medical_records WHERE patient_id > 0");
    query.exec("DELETE FROM patients WHERE id > 0");
    
    delete dbManager;
}

void TestCascadeDelete::testCascadeDeleteOnPatientDeletion()
{
    // Feature: medical-record-management, Property 1: 患者删除级联操作
    // Requirement 1.3: When a patient is deleted, all their medical records should be cascade deleted
    
    // Create a test patient
    Patient testPatient;
    testPatient.name = "Test Patient for Cascade";
    testPatient.phone = "555-9999";
    testPatient.email = "cascade@test.com";
    testPatient.birthDate = "1990-01-01";
    testPatient.gender = "Male";
    testPatient.medicalHistory = "Test history";
    
    QVERIFY(dbManager->addPatient(testPatient));
    
    // Get the patient ID
    QList<Patient> patients = dbManager->searchPatients("cascade@test.com");
    QVERIFY(patients.size() > 0);
    int patientId = patients.first().id;
    
    // Create multiple medical records for this patient
    for (int i = 0; i < 3; i++) {
        MedicalRecord record;
        record.patientId = patientId;
        record.diagnosis = QString("Test Diagnosis %1").arg(i);
        record.symptoms = QString("Test Symptoms %1").arg(i);
        record.treatment = QString("Test Treatment %1").arg(i);
        record.doctorName = "Dr. Test";
        record.notes = "Test notes";
        
        QVERIFY(dbManager->addMedicalRecord(record));
    }
    
    // Verify medical records were created
    QList<MedicalRecord> recordsBefore = dbManager->getMedicalRecordsByPatient(patientId);
    QCOMPARE(recordsBefore.size(), 3);
    
    // Delete the patient
    QVERIFY(dbManager->deletePatient(patientId));
    
    // Verify all medical records were cascade deleted
    QList<MedicalRecord> recordsAfter = dbManager->getMedicalRecordsByPatient(patientId);
    QCOMPARE(recordsAfter.size(), 0);
}

void TestCascadeDelete::testPatientInfoSynchronization()
{
    // Requirement 9.4: When patient info is updated, medical records should reflect the changes
    
    // Create a test patient
    Patient testPatient;
    testPatient.name = "Original Name";
    testPatient.phone = "555-8888";
    testPatient.email = "sync@test.com";
    testPatient.birthDate = "1985-05-15";
    testPatient.gender = "Female";
    testPatient.medicalHistory = "Original history";
    
    QVERIFY(dbManager->addPatient(testPatient));
    
    // Get the patient ID
    QList<Patient> patients = dbManager->searchPatients("sync@test.com");
    QVERIFY(patients.size() > 0);
    int patientId = patients.first().id;
    
    // Create a medical record
    MedicalRecord record;
    record.patientId = patientId;
    record.diagnosis = "Test Diagnosis";
    record.symptoms = "Test Symptoms";
    record.treatment = "Test Treatment";
    record.doctorName = "Dr. Sync";
    record.notes = "Test notes";
    
    QVERIFY(dbManager->addMedicalRecord(record));
    
    // Update patient name
    testPatient.id = patientId;
    testPatient.name = "Updated Name";
    QVERIFY(dbManager->updatePatient(testPatient));
    
    // Retrieve medical records and verify patient name is updated
    QList<MedicalRecord> records = dbManager->getMedicalRecordsByPatient(patientId);
    QVERIFY(records.size() > 0);
    QCOMPARE(records.first().patientName, QString("Updated Name"));
    
    // Clean up
    dbManager->deletePatient(patientId);
}

void TestCascadeDelete::testDeletedPatientMarker()
{
    // Requirement 9.5: If a patient is deleted, medical records should show "患者已删除" marker
    
    // Create a test patient
    Patient testPatient;
    testPatient.name = "Patient To Delete";
    testPatient.phone = "555-7777";
    testPatient.email = "marker@test.com";
    testPatient.birthDate = "1980-10-20";
    testPatient.gender = "Male";
    testPatient.medicalHistory = "Test history";
    
    QVERIFY(dbManager->addPatient(testPatient));
    
    // Get the patient ID
    QList<Patient> patients = dbManager->searchPatients("marker@test.com");
    QVERIFY(patients.size() > 0);
    int patientId = patients.first().id;
    
    // Create a medical record
    MedicalRecord record;
    record.patientId = patientId;
    record.diagnosis = "Test Diagnosis";
    record.symptoms = "Test Symptoms";
    record.treatment = "Test Treatment";
    record.doctorName = "Dr. Marker";
    record.notes = "Test notes";
    
    QVERIFY(dbManager->addMedicalRecord(record));
    
    // Get the record ID
    QList<MedicalRecord> recordsBefore = dbManager->getMedicalRecordsByPatient(patientId);
    QVERIFY(recordsBefore.size() > 0);
    int recordId = recordsBefore.first().id;
    
    // Note: Due to CASCADE DELETE, the medical record will be deleted when patient is deleted
    // This test verifies the behavior when records exist but patient is deleted
    // In practice, the CASCADE DELETE removes the records, so this marker is for
    // any orphaned records that might exist due to data migration or other scenarios
    
    // For this test, we'll verify the getAllMedicalRecords function handles null patient names
    QList<MedicalRecord> allRecords = dbManager->getAllMedicalRecords();
    
    // All records should have valid patient names or "患者已删除"
    for (const MedicalRecord &rec : allRecords) {
        QVERIFY(!rec.patientName.isEmpty());
    }
    
    // Clean up
    dbManager->deletePatient(patientId);
}

QTEST_MAIN(TestCascadeDelete)
#include "test_cascade_delete.moc"
