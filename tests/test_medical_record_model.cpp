#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include "../include/medicalrecordmodel.h"
#include "../include/databasemanager.h"

class TestMedicalRecordModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Test MedicalRecordModel basic functionality
    void testModelInitialization();
    void testColumnCount();
    void testHeaderData();
    void testDataFormatting();
    
    // Test CRUD operations
    void testInsertMedicalRecord();
    void testUpdateMedicalRecord();
    void testDeleteMedicalRecord();
    void testGetMedicalRecord();
    
    // Test data validation logic
    void testValidationRequiredFields();
    void testValidationFieldLengths();
    void testValidationPatientIdExists();
    void testValidationInvalidPatientId();
    
    // Test search and filtering
    void testSetFilter();
    void testRefreshData();
    
    // Test patient name display
    void testPatientNameDisplay();
    void testDeletedPatientDisplay();

private:
    DatabaseManager* dbManager;
    MedicalRecordModel* model;
    QString testConnectionName;
    int testPatientId;
    
    // Test configuration
    struct TestConfig {
        QString validHost = "localhost";
        QString validDatabase = "hospital_test_db";
        QString validUsername = "root";
        QString validPassword = "123456";
        int validPort = 3306;
    } config;
    
    void createTestDatabase();
    void dropTestDatabase();
    void insertTestPatient();
    void cleanupTestData();
    MedicalRecord createValidMedicalRecord();
    MedicalRecord createInvalidMedicalRecord();
};

void TestMedicalRecordModel::initTestCase()
{
    // Initialize test environment
    testConnectionName = "test_medical_record_model_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    
    // Create test database
    createTestDatabase();
    
    // Initialize database manager
    dbManager = &DatabaseManager::instance();
    
    qDebug() << "Test case initialized for MedicalRecordModel";
}

void TestMedicalRecordModel::cleanupTestCase()
{
    // Clean up
    if (model) {
        delete model;
        model = nullptr;
    }
    
    if (dbManager && dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
    
    // Drop test database
    dropTestDatabase();
    
    qDebug() << "Test case cleanup completed";
}

void TestMedicalRecordModel::init()
{
    // Connect to test database
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Create model instance
    model = new MedicalRecordModel(this);
    
    // Insert test patient
    insertTestPatient();
}

void TestMedicalRecordModel::cleanup()
{
    // Clean up test data
    cleanupTestData();
    
    // Delete model
    if (model) {
        delete model;
        model = nullptr;
    }
    
    // Disconnect database
    if (dbManager && dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
}

void TestMedicalRecordModel::createTestDatabase()
{
    QSqlDatabase setupDb = QSqlDatabase::addDatabase("QMYSQL", "setup_connection");
    setupDb.setHostName(config.validHost);
    setupDb.setUserName(config.validUsername);
    setupDb.setPassword(config.validPassword);
    setupDb.setPort(config.validPort);
    
    if (setupDb.open()) {
        QSqlQuery query(setupDb);
        query.exec(QString("DROP DATABASE IF EXISTS %1").arg(config.validDatabase));
        query.exec(QString("CREATE DATABASE %1 CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci").arg(config.validDatabase));
        setupDb.close();
    }
    
    QSqlDatabase::removeDatabase("setup_connection");
}

void TestMedicalRecordModel::dropTestDatabase()
{
    QSqlDatabase setupDb = QSqlDatabase::addDatabase("QMYSQL", "cleanup_connection");
    setupDb.setHostName(config.validHost);
    setupDb.setUserName(config.validUsername);
    setupDb.setPassword(config.validPassword);
    setupDb.setPort(config.validPort);
    
    if (setupDb.open()) {
        QSqlQuery query(setupDb);
        query.exec(QString("DROP DATABASE IF EXISTS %1").arg(config.validDatabase));
        setupDb.close();
    }
    
    QSqlDatabase::removeDatabase("cleanup_connection");
}

void TestMedicalRecordModel::insertTestPatient()
{
    Patient testPatient;
    testPatient.name = "Test Patient";
    testPatient.phone = "123456789";
    testPatient.email = "test@test.com";
    testPatient.address = "Test Address";
    testPatient.birthDate = "1990-01-01";
    testPatient.gender = "Male";
    testPatient.medicalHistory = "No known allergies";
    
    QVERIFY(dbManager->addPatient(testPatient));
    
    // Get the inserted patient ID
    QSqlQuery query;
    query.prepare("SELECT id FROM patients WHERE name = ? AND phone = ?");
    query.addBindValue(testPatient.name);
    query.addBindValue(testPatient.phone);
    QVERIFY(query.exec() && query.next());
    testPatientId = query.value(0).toInt();
    QVERIFY(testPatientId > 0);
}

void TestMedicalRecordModel::cleanupTestData()
{
    if (dbManager && dbManager->isConnected()) {
        QSqlQuery query;
        query.exec("DELETE FROM medical_records");
        query.exec("DELETE FROM patients");
    }
}

MedicalRecord TestMedicalRecordModel::createValidMedicalRecord()
{
    MedicalRecord record;
    record.patientId = testPatientId;
    record.diagnosis = "Test Diagnosis";
    record.symptoms = "Test Symptoms";
    record.treatment = "Test Treatment";
    record.doctorName = "Dr. Test";
    record.notes = "Test Notes";
    return record;
}

MedicalRecord TestMedicalRecordModel::createInvalidMedicalRecord()
{
    MedicalRecord record;
    record.patientId = -1; // Invalid patient ID
    record.diagnosis = ""; // Empty diagnosis
    record.doctorName = ""; // Empty doctor name
    return record;
}

void TestMedicalRecordModel::testModelInitialization()
{
    // Test model initialization
    QVERIFY(model != nullptr);
    QVERIFY(model->database().isValid());
    
    // Test that model is properly set up
    QCOMPARE(model->columnCount(), static_cast<int>(MedicalRecordModel::ColumnCount));
}

void TestMedicalRecordModel::testColumnCount()
{
    // Test column count matches enum
    QCOMPARE(model->columnCount(), static_cast<int>(MedicalRecordModel::ColumnCount));
    QCOMPARE(model->columnCount(), 9); // Based on the enum definition
}

void TestMedicalRecordModel::testHeaderData()
{
    // Test header data for all columns
    QCOMPARE(model->headerData(MedicalRecordModel::Id, Qt::Horizontal, Qt::DisplayRole).toString(), "病历ID");
    QCOMPARE(model->headerData(MedicalRecordModel::PatientId, Qt::Horizontal, Qt::DisplayRole).toString(), "患者ID");
    QCOMPARE(model->headerData(MedicalRecordModel::PatientName, Qt::Horizontal, Qt::DisplayRole).toString(), "患者姓名");
    QCOMPARE(model->headerData(MedicalRecordModel::CreatedDate, Qt::Horizontal, Qt::DisplayRole).toString(), "创建日期");
    QCOMPARE(model->headerData(MedicalRecordModel::Diagnosis, Qt::Horizontal, Qt::DisplayRole).toString(), "诊断");
    QCOMPARE(model->headerData(MedicalRecordModel::Symptoms, Qt::Horizontal, Qt::DisplayRole).toString(), "症状");
    QCOMPARE(model->headerData(MedicalRecordModel::Treatment, Qt::Horizontal, Qt::DisplayRole).toString(), "治疗方案");
    QCOMPARE(model->headerData(MedicalRecordModel::DoctorName, Qt::Horizontal, Qt::DisplayRole).toString(), "医生");
    QCOMPARE(model->headerData(MedicalRecordModel::Notes, Qt::Horizontal, Qt::DisplayRole).toString(), "备注");
}

void TestMedicalRecordModel::testDataFormatting()
{
    // Insert a test record first
    MedicalRecord record = createValidMedicalRecord();
    record.symptoms = "This is a very long symptom description that should be truncated when displayed in the table view to maintain readability";
    
    QVERIFY(model->insertMedicalRecord(record));
    
    // Test data formatting for long text
    if (model->rowCount() > 0) {
        QModelIndex symptomsIndex = model->index(0, MedicalRecordModel::Symptoms);
        QString displayText = model->data(symptomsIndex, Qt::DisplayRole).toString();
        
        // Should be truncated if longer than 50 characters
        if (record.symptoms.length() > 50) {
            QVERIFY(displayText.length() <= 50);
            QVERIFY(displayText.endsWith("..."));
        }
    }
}

void TestMedicalRecordModel::testInsertMedicalRecord()
{
    // Test inserting valid medical record
    MedicalRecord record = createValidMedicalRecord();
    
    int initialRowCount = model->rowCount();
    bool result = model->insertMedicalRecord(record);
    
    QVERIFY2(result, "Should successfully insert valid medical record");
    QVERIFY(model->rowCount() > initialRowCount);
    
    // Verify the record was inserted correctly
    if (model->rowCount() > 0) {
        MedicalRecord insertedRecord = model->getMedicalRecord(0);
        QCOMPARE(insertedRecord.patientId, record.patientId);
        QCOMPARE(insertedRecord.diagnosis, record.diagnosis);
        QCOMPARE(insertedRecord.doctorName, record.doctorName);
    }
}

void TestMedicalRecordModel::testUpdateMedicalRecord()
{
    // First insert a record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    // Get the inserted record
    QVERIFY(model->rowCount() > 0);
    MedicalRecord insertedRecord = model->getMedicalRecord(0);
    QVERIFY(insertedRecord.id > 0);
    
    // Update the record
    insertedRecord.diagnosis = "Updated Diagnosis";
    insertedRecord.treatment = "Updated Treatment";
    
    bool result = model->updateMedicalRecord(insertedRecord.id, insertedRecord);
    QVERIFY2(result, "Should successfully update medical record");
    
    // Verify the update
    MedicalRecord updatedRecord = model->getMedicalRecord(0);
    QCOMPARE(updatedRecord.diagnosis, QString("Updated Diagnosis"));
    QCOMPARE(updatedRecord.treatment, QString("Updated Treatment"));
}

void TestMedicalRecordModel::testDeleteMedicalRecord()
{
    // First insert a record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    // Get the inserted record
    QVERIFY(model->rowCount() > 0);
    int initialRowCount = model->rowCount();
    MedicalRecord insertedRecord = model->getMedicalRecord(0);
    QVERIFY(insertedRecord.id > 0);
    
    // Delete the record
    bool result = model->deleteMedicalRecord(insertedRecord.id);
    QVERIFY2(result, "Should successfully delete medical record");
    QVERIFY(model->rowCount() < initialRowCount);
}

void TestMedicalRecordModel::testGetMedicalRecord()
{
    // Insert a test record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    // Test getting valid record
    QVERIFY(model->rowCount() > 0);
    MedicalRecord retrievedRecord = model->getMedicalRecord(0);
    
    QVERIFY(retrievedRecord.id > 0);
    QCOMPARE(retrievedRecord.patientId, record.patientId);
    QCOMPARE(retrievedRecord.diagnosis, record.diagnosis);
    QCOMPARE(retrievedRecord.doctorName, record.doctorName);
    
    // Test getting invalid record (out of bounds)
    MedicalRecord invalidRecord = model->getMedicalRecord(-1);
    QCOMPARE(invalidRecord.id, -1);
    
    invalidRecord = model->getMedicalRecord(999);
    QCOMPARE(invalidRecord.id, -1);
}

void TestMedicalRecordModel::testValidationRequiredFields()
{
    // Test validation with missing required fields
    MedicalRecord record;
    
    // Missing patient ID
    record.patientId = 0;
    record.diagnosis = "Test Diagnosis";
    record.doctorName = "Dr. Test";
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with invalid patient ID");
    
    // Missing diagnosis
    record.patientId = testPatientId;
    record.diagnosis = "";
    record.doctorName = "Dr. Test";
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with empty diagnosis");
    
    // Missing doctor name
    record.patientId = testPatientId;
    record.diagnosis = "Test Diagnosis";
    record.doctorName = "";
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with empty doctor name");
}

void TestMedicalRecordModel::testValidationFieldLengths()
{
    MedicalRecord record = createValidMedicalRecord();
    
    // Test diagnosis length limit (500 characters)
    record.diagnosis = QString("a").repeated(501);
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with diagnosis longer than 500 characters");
    
    record.diagnosis = QString("a").repeated(500);
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with diagnosis exactly 500 characters");
    
    // Clean up for next test
    cleanupTestData();
    insertTestPatient();
    
    // Test symptoms length limit (1000 characters)
    record = createValidMedicalRecord();
    record.symptoms = QString("b").repeated(1001);
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with symptoms longer than 1000 characters");
    
    record.symptoms = QString("b").repeated(1000);
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with symptoms exactly 1000 characters");
    
    // Clean up for next test
    cleanupTestData();
    insertTestPatient();
    
    // Test treatment length limit (1000 characters)
    record = createValidMedicalRecord();
    record.treatment = QString("c").repeated(1001);
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with treatment longer than 1000 characters");
    
    record.treatment = QString("c").repeated(1000);
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with treatment exactly 1000 characters");
    
    // Clean up for next test
    cleanupTestData();
    insertTestPatient();
    
    // Test doctor name length limit (100 characters)
    record = createValidMedicalRecord();
    record.doctorName = QString("d").repeated(101);
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with doctor name longer than 100 characters");
    
    record.doctorName = QString("d").repeated(100);
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with doctor name exactly 100 characters");
    
    // Clean up for next test
    cleanupTestData();
    insertTestPatient();
    
    // Test notes length limit (2000 characters)
    record = createValidMedicalRecord();
    record.notes = QString("e").repeated(2001);
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with notes longer than 2000 characters");
    
    record.notes = QString("e").repeated(2000);
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with notes exactly 2000 characters");
}

void TestMedicalRecordModel::testValidationPatientIdExists()
{
    // Test with valid patient ID
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY2(model->insertMedicalRecord(record), "Should succeed with valid patient ID");
}

void TestMedicalRecordModel::testValidationInvalidPatientId()
{
    // Test with non-existent patient ID
    MedicalRecord record = createValidMedicalRecord();
    record.patientId = 99999; // Non-existent patient ID
    
    QVERIFY2(!model->insertMedicalRecord(record), "Should fail with non-existent patient ID");
}

void TestMedicalRecordModel::testSetFilter()
{
    // Insert multiple test records
    MedicalRecord record1 = createValidMedicalRecord();
    record1.diagnosis = "Hypertension";
    record1.doctorName = "Dr. Smith";
    QVERIFY(model->insertMedicalRecord(record1));
    
    MedicalRecord record2 = createValidMedicalRecord();
    record2.diagnosis = "Diabetes";
    record2.doctorName = "Dr. Johnson";
    QVERIFY(model->insertMedicalRecord(record2));
    
    int totalRecords = model->rowCount();
    QVERIFY(totalRecords >= 2);
    
    // Test filtering by diagnosis
    model->setFilter("Hypertension");
    QVERIFY(model->rowCount() > 0);
    QVERIFY(model->rowCount() < totalRecords);
    
    // Test filtering by doctor name
    model->setFilter("Dr. Johnson");
    QVERIFY(model->rowCount() > 0);
    
    // Test clearing filter
    model->setFilter("");
    QCOMPARE(model->rowCount(), totalRecords);
}

void TestMedicalRecordModel::testRefreshData()
{
    // Insert a test record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    int recordCount = model->rowCount();
    QVERIFY(recordCount > 0);
    
    // Manually insert another record directly to database
    QSqlQuery query;
    query.prepare("INSERT INTO medical_records (patient_id, diagnosis, doctor_name) VALUES (?, ?, ?)");
    query.addBindValue(testPatientId);
    query.addBindValue("Direct Insert Diagnosis");
    query.addBindValue("Dr. Direct");
    QVERIFY(query.exec());
    
    // Model should not reflect the change yet
    QCOMPARE(model->rowCount(), recordCount);
    
    // Refresh data
    model->refreshData();
    
    // Now model should reflect the new record
    QVERIFY(model->rowCount() > recordCount);
}

void TestMedicalRecordModel::testPatientNameDisplay()
{
    // Insert a test record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    // Verify patient name is displayed correctly
    QVERIFY(model->rowCount() > 0);
    MedicalRecord retrievedRecord = model->getMedicalRecord(0);
    QCOMPARE(retrievedRecord.patientName, QString("Test Patient"));
}

void TestMedicalRecordModel::testDeletedPatientDisplay()
{
    // Insert a test record
    MedicalRecord record = createValidMedicalRecord();
    QVERIFY(model->insertMedicalRecord(record));
    
    // Delete the patient (should cascade delete medical records due to foreign key)
    QVERIFY(dbManager->deletePatient(testPatientId));
    
    // Refresh model to see changes
    model->refreshData();
    
    // The medical record should be deleted due to cascade delete
    // But if we had a record with deleted patient, it would show "患者已删除"
    // Let's test this by manually inserting a record with non-existent patient
    
    // First, let's insert a record directly to database with invalid patient ID
    QSqlQuery query;
    query.exec("SET FOREIGN_KEY_CHECKS = 0"); // Temporarily disable foreign key checks
    query.prepare("INSERT INTO medical_records (patient_id, diagnosis, doctor_name) VALUES (?, ?, ?)");
    query.addBindValue(99999); // Non-existent patient ID
    query.addBindValue("Test Diagnosis");
    query.addBindValue("Dr. Test");
    QVERIFY(query.exec());
    query.exec("SET FOREIGN_KEY_CHECKS = 1"); // Re-enable foreign key checks
    
    // Refresh model
    model->refreshData();
    
    // Find the record with deleted patient
    bool foundDeletedPatientRecord = false;
    for (int i = 0; i < model->rowCount(); ++i) {
        MedicalRecord record = model->getMedicalRecord(i);
        if (record.patientName == "患者已删除") {
            foundDeletedPatientRecord = true;
            break;
        }
    }
    
    QVERIFY2(foundDeletedPatientRecord, "Should display '患者已删除' for records with deleted patients");
}

QTEST_MAIN(TestMedicalRecordModel)
#include "test_medical_record_model.moc"