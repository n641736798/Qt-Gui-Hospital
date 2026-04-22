#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include "../include/medicalrecordwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "Testing MedicalRecordWidget implementation...";
    
    try {
        // Create widget instance
        MedicalRecordWidget widget;
        
        // Test UI components exist
        QLineEdit *searchEdit = widget.findChild<QLineEdit*>("searchEdit");
        QPushButton *addButton = widget.findChild<QPushButton*>("addButton");
        QPushButton *editButton = widget.findChild<QPushButton*>("editButton");
        QPushButton *deleteButton = widget.findChild<QPushButton*>("deleteButton");
        QTableView *tableView = widget.findChild<QTableView*>("tableView");
        QLabel *messageLabel = widget.findChild<QLabel*>("messageLabel");
        QLabel *pageTitle = widget.findChild<QLabel*>("pageTitle");
        
        // Verify components exist
        if (!searchEdit) {
            qCritical() << "FAIL: Search edit not found";
            return 1;
        }
        
        if (!addButton) {
            qCritical() << "FAIL: Add button not found";
            return 1;
        }
        
        if (!editButton) {
            qCritical() << "FAIL: Edit button not found";
            return 1;
        }
        
        if (!deleteButton) {
            qCritical() << "FAIL: Delete button not found";
            return 1;
        }
        
        if (!tableView) {
            qCritical() << "FAIL: Table view not found";
            return 1;
        }
        
        if (!messageLabel) {
            qCritical() << "FAIL: Message label not found";
            return 1;
        }
        
        if (!pageTitle) {
            qCritical() << "FAIL: Page title not found";
            return 1;
        }
        
        // Test component properties
        if (searchEdit->placeholderText() != "按患者姓名、诊断或医生搜索...") {
            qCritical() << "FAIL: Search edit placeholder text incorrect";
            return 1;
        }
        
        if (addButton->text() != "添加病历") {
            qCritical() << "FAIL: Add button text incorrect";
            return 1;
        }
        
        if (editButton->text() != "编辑") {
            qCritical() << "FAIL: Edit button text incorrect";
            return 1;
        }
        
        if (deleteButton->text() != "删除") {
            qCritical() << "FAIL: Delete button text incorrect";
            return 1;
        }
        
        if (pageTitle->text() != "病历信息管理") {
            qCritical() << "FAIL: Page title text incorrect";
            return 1;
        }
        
        // Test button states
        if (!addButton->isEnabled()) {
            qCritical() << "FAIL: Add button should be enabled initially";
            return 1;
        }
        
        if (editButton->isEnabled()) {
            qCritical() << "FAIL: Edit button should be disabled initially";
            return 1;
        }
        
        if (deleteButton->isEnabled()) {
            qCritical() << "FAIL: Delete button should be disabled initially";
            return 1;
        }
        
        // Test table view configuration
        if (tableView->selectionBehavior() != QAbstractItemView::SelectRows) {
            qCritical() << "FAIL: Table view selection behavior incorrect";
            return 1;
        }
        
        if (tableView->selectionMode() != QAbstractItemView::SingleSelection) {
            qCritical() << "FAIL: Table view selection mode incorrect";
            return 1;
        }
        
        if (!tableView->isSortingEnabled()) {
            qCritical() << "FAIL: Table view sorting should be enabled";
            return 1;
        }
        
        if (!tableView->alternatingRowColors()) {
            qCritical() << "FAIL: Table view should have alternating row colors";
            return 1;
        }
        
        qDebug() << "SUCCESS: All MedicalRecordWidget tests passed!";
        qDebug() << "- UI components created correctly";
        qDebug() << "- Component properties set correctly";
        qDebug() << "- Button states initialized correctly";
        qDebug() << "- Table view configured correctly";
        
        return 0;
        
    } catch (const std::exception &e) {
        qCritical() << "EXCEPTION:" << e.what();
        return 1;
    } catch (...) {
        qCritical() << "UNKNOWN EXCEPTION occurred";
        return 1;
    }
}