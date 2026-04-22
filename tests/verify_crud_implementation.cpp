#include <QtTest/QtTest>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include "../include/medicalrecordwidget.h"

class TestCRUDImplementation : public QObject
{
    Q_OBJECT

private slots:
    void testCRUDMethodsExist();
    void testUIComponentsExist();
    void testButtonConnections();

private:
    void testMethodExists(QObject* obj, const char* methodName);
};

void TestCRUDImplementation::testCRUDMethodsExist()
{
    // Create widget without database connection
    MedicalRecordWidget widget;
    
    // Test that CRUD methods exist and are callable
    testMethodExists(&widget, "onAddRecord()");
    testMethodExists(&widget, "onEditRecord()");
    testMethodExists(&widget, "onDeleteRecord()");
    testMethodExists(&widget, "refreshData()");
    testMethodExists(&widget, "onSearchChanged(QString)");
}

void TestCRUDImplementation::testUIComponentsExist()
{
    MedicalRecordWidget widget;
    
    // Test that all required UI components exist
    QLineEdit *searchEdit = widget.findChild<QLineEdit*>("searchEdit");
    QVERIFY2(searchEdit != nullptr, "Search edit should exist");
    
    QPushButton *addButton = widget.findChild<QPushButton*>("addButton");
    QVERIFY2(addButton != nullptr, "Add button should exist");
    
    QPushButton *editButton = widget.findChild<QPushButton*>("editButton");
    QVERIFY2(editButton != nullptr, "Edit button should exist");
    
    QPushButton *deleteButton = widget.findChild<QPushButton*>("deleteButton");
    QVERIFY2(deleteButton != nullptr, "Delete button should exist");
    
    QTableView *tableView = widget.findChild<QTableView*>("tableView");
    QVERIFY2(tableView != nullptr, "Table view should exist");
    
    QLabel *messageLabel = widget.findChild<QLabel*>("messageLabel");
    QVERIFY2(messageLabel != nullptr, "Message label should exist");
}

void TestCRUDImplementation::testButtonConnections()
{
    MedicalRecordWidget widget;
    
    QPushButton *addButton = widget.findChild<QPushButton*>("addButton");
    QPushButton *editButton = widget.findChild<QPushButton*>("editButton");
    QPushButton *deleteButton = widget.findChild<QPushButton*>("deleteButton");
    
    QVERIFY(addButton != nullptr);
    QVERIFY(editButton != nullptr);
    QVERIFY(deleteButton != nullptr);
    
    // Test button states - add should be enabled, others disabled initially
    QVERIFY2(addButton->isEnabled(), "Add button should be enabled");
    QVERIFY2(!editButton->isEnabled(), "Edit button should be disabled initially");
    QVERIFY2(!deleteButton->isEnabled(), "Delete button should be disabled initially");
}

void TestCRUDImplementation::testMethodExists(QObject* obj, const char* methodName)
{
    const QMetaObject* metaObject = obj->metaObject();
    bool found = false;
    
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);
        if (QString(method.methodSignature()).contains(QString(methodName).split('(')[0])) {
            found = true;
            break;
        }
    }
    
    QVERIFY2(found, QString("Method %1 should exist").arg(methodName).toLocal8Bit().constData());
}

QTEST_MAIN(TestCRUDImplementation)
#include "verify_crud_implementation.moc"