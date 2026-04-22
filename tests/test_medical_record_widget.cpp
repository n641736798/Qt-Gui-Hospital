#include <QtTest/QtTest>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QHeaderView>
#include "../include/medicalrecordwidget.h"
#include "../include/databasemanager.h"

class TestMedicalRecordWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Test UI components
    void testUIComponents();
    void testButtonStates();
    void testTableViewConfiguration();
    void testSearchFunctionality();
    void testMessageDisplay();

private:
    MedicalRecordWidget* widget;
    DatabaseManager* dbManager;
    QString testConnectionName;
    int testPatientId;
    
    // Test configuration
    struct TestConfig {
        QString validHost = "localhost";
        QString validDatabase = "hospital_test_widget_db";
        QString validUsername = "root";
        QString validPassword = "123456";
        int validPort = 3306;
    } config;
    
    void createTestDatabase();
    void dropTestDatabase();
    void insertTestPatient();
    void insertTestMedicalRecords();
    void cleanupTestData();
};

void TestMedicalRecordWidget::initTestCase()
{
    // Initialize test environment
    testConnectionName = "test_medical_record_widget_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    
    // Create test database
    createTestDatabase();
    
    // Initialize database manager
    dbManager = &DatabaseManager::instance();
    
    qDebug() << "Test case initialized for MedicalRecordWidget";
}

void TestMedicalRecordWidget::cleanupTestCase()
{
    // Clean up
    if (widget) {
        delete widget;
        widget = nullptr;
    }
    
    if (dbManager && dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
    
    // Drop test database
    dropTestDatabase();
    
    qDebug() << "Test case cleanup completed";
}

void TestMedicalRecordWidget::init()
{
    // Connect to test database
    QVERIFY(dbManager->connectToDatabase(
        config.validHost,
        config.validDatabase,
        config.validUsername,
        config.validPassword,
        config.validPort
    ));
    
    // Insert test patient
    insertTestPatient();
    
    // Create widget instance
    widget = new MedicalRecordWidget();
}

void TestMedicalRecordWidget::cleanup()
{
    // Clean up test data
    cleanupTestData();
    
    // Delete widget
    if (widget) {
        delete widget;
        widget = nullptr;
    }
    
    // Disconnect database
    if (dbManager && dbManager->isConnected()) {
        dbManager->disconnectFromDatabase();
    }
}

void TestMedicalRecordWidget::createTestDatabase()
{
    QSqlDatabase setupDb = QSqlDatabase::addDatabase("QMYSQL", "setup_widget_connection");
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
    
    QSqlDatabase::removeDatabase("setup_widget_connection");
}

void TestMedicalRecordWidget::dropTestDatabase()
{
    QSqlDatabase setupDb = QSqlDatabase::addDatabase("QMYSQL", "cleanup_widget_connection");
    setupDb.setHostName(config.validHost);
    setupDb.setUserName(config.validUsername);
    setupDb.setPassword(config.validPassword);
    setupDb.setPort(config.validPort);
    
    if (setupDb.open()) {
        QSqlQuery query(setupDb);
        query.exec(QString("DROP DATABASE IF EXISTS %1").arg(config.validDatabase));
        setupDb.close();
    }
    
    QSqlDatabase::removeDatabase("cleanup_widget_connection");
}

void TestMedicalRecordWidget::insertTestPatient()
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

void TestMedicalRecordWidget::insertTestMedicalRecords()
{
    // Insert multiple test records for search testing
    MedicalRecord record1;
    record1.patientId = testPatientId;
    record1.diagnosis = "Hypertension";
    record1.symptoms = "High blood pressure";
    record1.treatment = "Medication";
    record1.doctorName = "Dr. Smith";
    record1.notes = "Regular checkups needed";
    QVERIFY(dbManager->addMedicalRecord(record1));
    
    MedicalRecord record2;
    record2.patientId = testPatientId;
    record2.diagnosis = "Diabetes";
    record2.symptoms = "High blood sugar";
    record2.treatment = "Insulin therapy";
    record2.doctorName = "Dr. Johnson";
    record2.notes = "Monitor blood sugar levels";
    QVERIFY(dbManager->addMedicalRecord(record2));
    
    MedicalRecord record3;
    record3.patientId = testPatientId;
    record3.diagnosis = "Common Cold";
    record3.symptoms = "Runny nose, cough";
    record3.treatment = "Rest and fluids";
    record3.doctorName = "Dr. Smith";
    record3.notes = "Should recover in a few days";
    QVERIFY(dbManager->addMedicalRecord(record3));
}

void TestMedicalRecordWidget::cleanupTestData()
{
    if (dbManager && dbManager->isConnected()) {
        QSqlQuery query;
        query.exec("DELETE FROM medical_records");
        query.exec("DELETE FROM patients");
    }
}

void TestMedicalRecordWidget::testUIComponents()
{
    // Test that all required UI components exist
    QVERIFY2(widget != nullptr, "Widget should be created successfully");
    
    // Test search edit
    QLineEdit *searchEdit = widget->findChild<QLineEdit*>("searchEdit");
    QVERIFY2(searchEdit != nullptr, "Search edit should exist");
    QCOMPARE(searchEdit->placeholderText(), QString("按患者姓名、诊断或医生搜索..."));
    
    // Test buttons
    QPushButton *addButton = widget->findChild<QPushButton*>("addButton");
    QVERIFY2(addButton != nullptr, "Add button should exist");
    QCOMPARE(addButton->text(), QString("添加病历"));
    
    QPushButton *editButton = widget->findChild<QPushButton*>("editButton");
    QVERIFY2(editButton != nullptr, "Edit button should exist");
    QCOMPARE(editButton->text(), QString("编辑"));
    
    QPushButton *deleteButton = widget->findChild<QPushButton*>("deleteButton");
    QVERIFY2(deleteButton != nullptr, "Delete button should exist");
    QCOMPARE(deleteButton->text(), QString("删除"));
    
    // Test table view
    QTableView *tableView = widget->findChild<QTableView*>("tableView");
    QVERIFY2(tableView != nullptr, "Table view should exist");
    
    // Test message label
    QLabel *messageLabel = widget->findChild<QLabel*>("messageLabel");
    QVERIFY2(messageLabel != nullptr, "Message label should exist");
    
    // Test page title
    QLabel *pageTitle = widget->findChild<QLabel*>("pageTitle");
    QVERIFY2(pageTitle != nullptr, "Page title should exist");
    QCOMPARE(pageTitle->text(), QString("病历信息管理"));
}

void TestMedicalRecordWidget::testButtonStates()
{
    QPushButton *addButton = widget->findChild<QPushButton*>("addButton");
    QPushButton *editButton = widget->findChild<QPushButton*>("editButton");
    QPushButton *deleteButton = widget->findChild<QPushButton*>("deleteButton");
    
    QVERIFY(addButton != nullptr);
    QVERIFY(editButton != nullptr);
    QVERIFY(deleteButton != nullptr);
    
    // Initially, add button should be enabled, edit and delete should be disabled
    QVERIFY2(addButton->isEnabled(), "Add button should be enabled initially");
    QVERIFY2(!editButton->isEnabled(), "Edit button should be disabled initially");
    QVERIFY2(!deleteButton->isEnabled(), "Delete button should be disabled initially");
}

void TestMedicalRecordWidget::testTableViewConfiguration()
{
    QTableView *tableView = widget->findChild<QTableView*>("tableView");
    QVERIFY(tableView != nullptr);
    
    // Test table view properties
    QVERIFY2(tableView->alternatingRowColors(), "Table should have alternating row colors");
    QCOMPARE(tableView->selectionBehavior(), QAbstractItemView::SelectRows);
    QCOMPARE(tableView->selectionMode(), QAbstractItemView::SingleSelection);
    QVERIFY2(tableView->isSortingEnabled(), "Table should have sorting enabled");
    
    // Test that model is set
    QVERIFY2(tableView->model() != nullptr, "Table view should have a model");
    
    // Test header configuration
    QHeaderView *header = tableView->horizontalHeader();
    QVERIFY2(header != nullptr, "Table should have a horizontal header");
    QVERIFY2(header->stretchLastSection(), "Last section should be stretched");
}

void TestMedicalRecordWidget::testSearchFunctionality()
{
    // Insert test data first
    insertTestMedicalRecords();
    
    // Refresh widget data
    widget->refreshData();
    
    QLineEdit *searchEdit = widget->findChild<QLineEdit*>("searchEdit");
    QTableView *tableView = widget->findChild<QTableView*>("tableView");
    
    QVERIFY(searchEdit != nullptr);
    QVERIFY(tableView != nullptr);
    
    // Get initial row count
    int initialRowCount = tableView->model()->rowCount();
    QVERIFY2(initialRowCount >= 3, "Should have at least 3 test records");
    
    // Test search by diagnosis
    searchEdit->setText("Hypertension");
    QTest::qWait(100); // Wait for search to process
    
    int filteredRowCount = tableView->model()->rowCount();
    QVERIFY2(filteredRowCount > 0, "Should find records matching 'Hypertension'");
    QVERIFY2(filteredRowCount < initialRowCount, "Filtered results should be less than total");
    
    // Test search by doctor name
    searchEdit->setText("Dr. Smith");
    QTest::qWait(100);
    
    filteredRowCount = tableView->model()->rowCount();
    QVERIFY2(filteredRowCount > 0, "Should find records for 'Dr. Smith'");
    
    // Test clearing search
    searchEdit->clear();
    QTest::qWait(100);
    
    int clearedRowCount = tableView->model()->rowCount();
    QCOMPARE(clearedRowCount, initialRowCount);
}

void TestMedicalRecordWidget::testMessageDisplay()
{
    QLabel *messageLabel = widget->findChild<QLabel*>("messageLabel");
    QTableView *tableView = widget->findChild<QTableView*>("tableView");
    
    QVERIFY(messageLabel != nullptr);
    QVERIFY(tableView != nullptr);
    
    // Initially, message should be hidden if there are no records
    // or visible if there are no records
    if (tableView->model()->rowCount() == 0) {
        QVERIFY2(messageLabel->isVisible(), "Message should be visible when no records");
        QVERIFY2(!tableView->isVisible(), "Table should be hidden when no records");
        QCOMPARE(messageLabel->text(), QString("暂无病历记录"));
    } else {
        QVERIFY2(!messageLabel->isVisible(), "Message should be hidden when records exist");
        QVERIFY2(tableView->isVisible(), "Table should be visible when records exist");
    }
}

QTEST_MAIN(TestMedicalRecordWidget)
#include "test_medical_record_widget.moc"