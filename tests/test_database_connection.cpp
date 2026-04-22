#include <QtTest/QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../include/databasemanager.h"

class TestDatabaseConnection : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Test MySQL connection establishment and failure scenarios
    void testSuccessfulConnection();
    void testConnectionWithInvalidHost();
    void testConnectionWithInvalidCredentials();
    void testConnectionWithInvalidDatabase();
    void testConnectionWithInvalidPort();
    void testConnectionTimeout();
    
    // Test table creation and index verification
    void testTableCreation();
    void testMedicalRecordTableCreation();
    void testTableIndexes();
    void testForeignKeyConstraints();
    
    // Test connection configuration error handling
    void testConnectionErrorHandling();
    void testLastErrorMessage();
    void testConnectionStatus();
    void testConnectionRetry();

private:
    DatabaseManager* dbManager;
    QString testConnectionName;
    
    // Test configuration
    struct TestConfig {
        QString validHost = "localhost";
        QString validDatabase = "hospital_test_db";
        QString validUsername = "root";
        QString validPassword = "123456";
        int validPort = 3306;
        
        QString invalidHost = "invalid_host_12345";
        QString invalidDatabase = "nonexistent_db_12345";
        QString invalidUsername = "invalid_user_12345";
        QString invalidPassword = "invalid_pass_12345";
        int invalidPort = 9999;
    } config;
    
    void createTestDatabase();
    void dropTestDatabase();
    bool tableExists(const QString &tableName);
    bool indexExists(const QString &tableName, const QString &indexName);
    bool foreignKeyExists(const QString &tableName, const QString &constraintName);
};

void TestDatabaseConnection::initTestCase()
{
    // Initialize test environment
    testConnectionName = "test_connection_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    
    // Create test database first
    createTestDatabase();
    
    qDebug() << "Test case initialized with connection:" << testConnectionName;
}

void TestDatabaseConnection::cleanupTestCase()
{
    // Clean up test database
    dropTestDatabase();
    
    qDebug() << "Test case cleanup completed";
}

void TestDatabaseConnection::init()
{
    // Create fresh DatabaseManager instance for each test
    dbManager = &DatabaseManager::instance();
    
    // Ensure clean state
    if (dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
}

void TestDatabaseConnection::cleanup()
{
    // Disconnect after each test
    if (dbManager && dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
}

void TestDatabaseConnection::createTestDatabase()
{
    // Create test database using system connection
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

void TestDatabaseConnection::dropTestDatabase()
{
    // Drop test database
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

void TestDatabaseConnection::testSuccessfulConnection()
{
    // Test successful MySQL connection
    bool result = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    QVERIFY2(result, "Should successfully connect to valid MySQL database");
    QVERIFY2(dbManager->isConnected(), "Database should be connected");
    QVERIFY2(dbManager->testConnection(), "Connection test should pass");
    QVERIFY2(dbManager->getLastError().isEmpty(), "No error should be present after successful connection");
}

void TestDatabaseConnection::testConnectionWithInvalidHost()
{
    // Test connection failure with invalid host
    bool result = dbManager->connectToDatabase(
        config.invalidHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    QVERIFY2(!result, "Should fail to connect to invalid host");
    QVERIFY2(!dbManager->isConnected(), "Database should not be connected");
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error message should be present");
    QVERIFY2(dbManager->getLastError().contains("连接失败"), "Error should indicate connection failure");
}

void TestDatabaseConnection::testConnectionWithInvalidCredentials()
{
    // Test connection failure with invalid username
    bool result = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.invalidUsername,
        config.invalidPassword,
        config.validPort
    );
    
    QVERIFY2(!result, "Should fail to connect with invalid credentials");
    QVERIFY2(!dbManager->isConnected(), "Database should not be connected");
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error message should be present");
}

void TestDatabaseConnection::testConnectionWithInvalidDatabase()
{
    // Test connection failure with non-existent database
    bool result = dbManager->connectToDatabase(
        config.validHost,
        config.invalidDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    QVERIFY2(!result, "Should fail to connect to non-existent database");
    QVERIFY2(!dbManager->isConnected(), "Database should not be connected");
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error message should be present");
}

void TestDatabaseConnection::testConnectionWithInvalidPort()
{
    // Test connection failure with invalid port
    bool result = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.invalidPort
    );
    
    QVERIFY2(!result, "Should fail to connect to invalid port");
    QVERIFY2(!dbManager->isConnected(), "Database should not be connected");
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error message should be present");
}

void TestDatabaseConnection::testConnectionTimeout()
{
    // Test connection timeout scenario
    // Using a valid IP that doesn't respond to simulate timeout
    bool result = dbManager->connectToDatabase(
        "192.168.255.255", // Non-routable IP to simulate timeout
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    QVERIFY2(!result, "Should fail to connect due to timeout");
    QVERIFY2(!dbManager->isConnected(), "Database should not be connected");
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error message should be present");
}

void TestDatabaseConnection::testTableCreation()
{
    // First establish connection
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Verify patients table was created
    QVERIFY2(tableExists("patients"), "Patients table should be created");
    
    // Verify table structure
    QSqlQuery query;
    QVERIFY(query.exec("DESCRIBE patients"));
    
    QStringList expectedColumns = {"id", "name", "phone", "email", "address", "birth_date", "gender", "medical_history", "created_at", "updated_at"};
    QStringList actualColumns;
    
    while (query.next()) {
        actualColumns << query.value(0).toString();
    }
    
    for (const QString &column : expectedColumns) {
        QVERIFY2(actualColumns.contains(column), QString("Column %1 should exist in patients table").arg(column).toLocal8Bit());
    }
}

void TestDatabaseConnection::testMedicalRecordTableCreation()
{
    // First establish connection
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Verify medical_records table was created
    QVERIFY2(tableExists("medical_records"), "Medical records table should be created");
    
    // Verify table structure
    QSqlQuery query;
    QVERIFY(query.exec("DESCRIBE medical_records"));
    
    QStringList expectedColumns = {"id", "patient_id", "created_date", "diagnosis", "symptoms", "treatment", "doctor_name", "notes"};
    QStringList actualColumns;
    
    while (query.next()) {
        actualColumns << query.value(0).toString();
    }
    
    for (const QString &column : expectedColumns) {
        QVERIFY2(actualColumns.contains(column), QString("Column %1 should exist in medical_records table").arg(column).toLocal8Bit());
    }
}

void TestDatabaseConnection::testTableIndexes()
{
    // First establish connection
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Test patients table indexes
    QVERIFY2(indexExists("patients", "idx_name"), "Name index should exist on patients table");
    QVERIFY2(indexExists("patients", "idx_phone"), "Phone index should exist on patients table");
    
    // Test medical_records table indexes
    QVERIFY2(indexExists("medical_records", "idx_patient_id"), "Patient ID index should exist on medical_records table");
    QVERIFY2(indexExists("medical_records", "idx_created_date"), "Created date index should exist on medical_records table");
    QVERIFY2(indexExists("medical_records", "idx_doctor_name"), "Doctor name index should exist on medical_records table");
    QVERIFY2(indexExists("medical_records", "idx_patient_date"), "Composite patient-date index should exist on medical_records table");
}

void TestDatabaseConnection::testForeignKeyConstraints()
{
    // First establish connection
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Test foreign key constraint exists
    QSqlQuery query;
    QVERIFY(query.exec(R"(
        SELECT CONSTRAINT_NAME 
        FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE 
        WHERE TABLE_SCHEMA = DATABASE() 
        AND TABLE_NAME = 'medical_records' 
        AND REFERENCED_TABLE_NAME = 'patients'
    )"));
    
    QVERIFY2(query.next(), "Foreign key constraint should exist between medical_records and patients");
    
    // Test cascade delete behavior
    // Insert test patient
    QVERIFY(query.exec("INSERT INTO patients (name, phone, email, gender) VALUES ('Test Patient', '123456789', 'test@test.com', 'Male')"));
    int patientId = query.lastInsertId().toInt();
    
    // Insert test medical record
    query.prepare("INSERT INTO medical_records (patient_id, diagnosis, doctor_name) VALUES (?, 'Test Diagnosis', 'Test Doctor')");
    query.addBindValue(patientId);
    QVERIFY(query.exec());
    
    // Verify record exists
    query.prepare("SELECT COUNT(*) FROM medical_records WHERE patient_id = ?");
    query.addBindValue(patientId);
    QVERIFY(query.exec() && query.next());
    QVERIFY2(query.value(0).toInt() > 0, "Medical record should exist before patient deletion");
    
    // Delete patient (should cascade delete medical records)
    query.prepare("DELETE FROM patients WHERE id = ?");
    query.addBindValue(patientId);
    QVERIFY(query.exec());
    
    // Verify medical record was cascade deleted
    query.prepare("SELECT COUNT(*) FROM medical_records WHERE patient_id = ?");
    query.addBindValue(patientId);
    QVERIFY(query.exec() && query.next());
    QVERIFY2(query.value(0).toInt() == 0, "Medical record should be cascade deleted when patient is deleted");
}

void TestDatabaseConnection::testConnectionErrorHandling()
{
    // Test error handling for various connection scenarios
    QString originalError = dbManager->getLastError();
    
    // Test with invalid host
    dbManager->connectToDatabase(config.invalidHost, config.validDatabase, config.validUsername, config.validPassword);
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error should be set for invalid host");
    
    // Test with invalid credentials
    dbManager->connectToDatabase(config.validHost, config.validDatabase, config.invalidUsername, config.invalidPassword);
    QVERIFY2(!dbManager->getLastError().isEmpty(), "Error should be set for invalid credentials");
    
    // Test connection status after errors
    QVERIFY2(!dbManager->isConnected(), "Should not be connected after connection errors");
    QVERIFY2(!dbManager->testConnection(), "Connection test should fail when not connected");
}

void TestDatabaseConnection::testLastErrorMessage()
{
    // Test error message functionality
    QString initialError = dbManager->getLastError();
    
    // Attempt invalid connection
    bool result = dbManager->connectToDatabase(
        config.invalidHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword
    );
    
    QVERIFY2(!result, "Connection should fail");
    
    QString errorAfterFailure = dbManager->getLastError();
    QVERIFY2(!errorAfterFailure.isEmpty(), "Error message should be set after connection failure");
    QVERIFY2(errorAfterFailure.contains("连接失败"), "Error message should indicate connection failure");
    
    // Test successful connection clears error
    result = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    if (result) {
        // Error should be cleared on successful connection
        QVERIFY2(dbManager->getLastError().isEmpty() || !dbManager->getLastError().contains("连接失败"), 
                "Error should be cleared or not contain connection failure after successful connection");
    }
}

void TestDatabaseConnection::testConnectionStatus()
{
    // Test connection status methods
    QVERIFY2(!dbManager->isConnected(), "Should not be connected initially");
    QVERIFY2(!dbManager->testConnection(), "Connection test should fail when not connected");
    
    // Connect and test status
    bool result = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    if (result) {
        QVERIFY2(dbManager->isConnected(), "Should be connected after successful connection");
        QVERIFY2(dbManager->testConnection(), "Connection test should pass when connected");
        
        // Disconnect and test status
        dbManager->disconnectFromDatabase();
        QVERIFY2(!dbManager->isConnected(), "Should not be connected after disconnection");
        QVERIFY2(!dbManager->testConnection(), "Connection test should fail after disconnection");
    }
}

void TestDatabaseConnection::testConnectionRetry()
{
    // Test connection retry scenarios
    // First, fail with invalid credentials
    bool result1 = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.invalidUsername,
        config.invalidPassword,
        config.validPort
    );
    
    QVERIFY2(!result1, "First connection attempt should fail");
    QVERIFY2(!dbManager->isConnected(), "Should not be connected after failed attempt");
    
    // Then succeed with valid credentials
    bool result2 = dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    );
    
    QVERIFY2(result2, "Second connection attempt should succeed");
    QVERIFY2(dbManager->isConnected(), "Should be connected after successful retry");
}

bool TestDatabaseConnection::tableExists(const QString &tableName)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = ?");
    query.addBindValue(tableName);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool TestDatabaseConnection::indexExists(const QString &tableName, const QString &indexName)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT COUNT(*) 
        FROM INFORMATION_SCHEMA.STATISTICS 
        WHERE TABLE_SCHEMA = DATABASE() 
        AND TABLE_NAME = ? 
        AND INDEX_NAME = ?
    )");
    query.addBindValue(tableName);
    query.addBindValue(indexName);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool TestDatabaseConnection::foreignKeyExists(const QString &tableName, const QString &constraintName)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT COUNT(*) 
        FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE 
        WHERE TABLE_SCHEMA = DATABASE() 
        AND TABLE_NAME = ? 
        AND CONSTRAINT_NAME = ?
        AND REFERENCED_TABLE_NAME IS NOT NULL
    )");
    query.addBindValue(tableName);
    query.addBindValue(constraintName);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QTEST_MAIN(TestDatabaseConnection)
#include "test_database_connection.moc"