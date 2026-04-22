# Database Connection Unit Tests

This directory contains unit tests for the DatabaseManager class, specifically testing MySQL connection functionality as required by task 1.1.

## Test Coverage

The tests cover the following scenarios as specified in the requirements:

### MySQL Connection Establishment and Failure Scenarios
- ✅ Successful connection to valid MySQL database
- ✅ Connection failure with invalid host
- ✅ Connection failure with invalid credentials (username/password)
- ✅ Connection failure with non-existent database
- ✅ Connection failure with invalid port
- ✅ Connection timeout scenarios

### Table Creation and Index Verification
- ✅ Patients table creation with proper structure
- ✅ Medical records table creation with proper structure
- ✅ Index verification for all required indexes:
  - `idx_name` and `idx_phone` on patients table
  - `idx_patient_id`, `idx_created_date`, `idx_doctor_name`, `idx_patient_date` on medical_records table
- ✅ Foreign key constraint verification and cascade delete testing

### Connection Configuration Error Handling
- ✅ Error message generation for various failure scenarios
- ✅ Error message clearing on successful connection
- ✅ Connection status tracking (`isConnected()`, `testConnection()`)
- ✅ Connection retry scenarios
- ✅ Proper disconnection handling

## Requirements Validation

This test suite validates the following requirements:

- **Requirement 1.1**: Database connection establishment and error handling
- **Requirement 1.2**: Table creation and database schema validation

## Prerequisites

Before running the tests, ensure:

1. **MySQL Server**: MySQL server is running and accessible
2. **Database Credentials**: Update the test configuration in `test_database_connection.cpp` if needed:
   ```cpp
   QString validHost = "localhost";
   QString validUsername = "root"; 
   QString validPassword = "123456";
   int validPort = 3306;
   ```
3. **Qt Test Framework**: Qt Test module is available
4. **MySQL Driver**: Qt MySQL driver (QMYSQL) is installed

## Running the Tests

### Method 1: Using the Batch Script (Windows)
```bash
cd tests
run_tests.bat
```

### Method 2: Manual Build and Run
```bash
cd tests
qmake tests.pro
make
../build/tests/database_connection_tests.exe
```

### Method 3: Using Qt Creator
1. Open `tests.pro` in Qt Creator
2. Build the project
3. Run the tests

## Test Database

The tests automatically create and manage a test database named `hospital_test_db`. This database is:
- Created before running tests
- Populated with test data during tests
- Cleaned up after tests complete

**Note**: The test database is separate from your main application database and will not affect your production data.

## Expected Output

Successful test run should show output similar to:
```
********* Start testing of TestDatabaseConnection *********
Config: Using QtTest library
PASS   : TestDatabaseConnection::initTestCase()
PASS   : TestDatabaseConnection::testSuccessfulConnection()
PASS   : TestDatabaseConnection::testConnectionWithInvalidHost()
PASS   : TestDatabaseConnection::testConnectionWithInvalidCredentials()
...
PASS   : TestDatabaseConnection::cleanupTestCase()
Totals: XX passed, 0 failed, 0 skipped, 0 blacklisted
********* Finished testing of TestDatabaseConnection *********
```

## Troubleshooting

### Common Issues

1. **MySQL Driver Not Found**
   ```
   Error: QMYSQL driver not available
   ```
   **Solution**: Install Qt MySQL driver or rebuild Qt with MySQL support

2. **Connection Refused**
   ```
   Error: Can't connect to MySQL server
   ```
   **Solution**: Ensure MySQL server is running and accessible

3. **Access Denied**
   ```
   Error: Access denied for user
   ```
   **Solution**: Check username/password in test configuration

4. **Database Creation Failed**
   ```
   Error: Can't create database
   ```
   **Solution**: Ensure the MySQL user has CREATE privileges

### Debug Mode

To run tests with detailed debug output:
```bash
database_connection_tests.exe -v2
```

## Test Structure

The test class `TestDatabaseConnection` follows Qt Test framework conventions:
- `initTestCase()`: Set up test environment and create test database
- `cleanupTestCase()`: Clean up test database
- `init()`: Initialize before each test method
- `cleanup()`: Clean up after each test method
- Individual test methods: Test specific functionality

Each test method uses `QVERIFY2()` assertions with descriptive error messages to ensure clear failure reporting.