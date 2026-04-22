#include <iostream>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "=== 简化数据库连接测试 ===" << std::endl;
    
    // 检查MySQL驱动是否可用
    QStringList drivers = QSqlDatabase::drivers();
    std::cout << "可用的数据库驱动: ";
    for (const QString &driver : drivers) {
        std::cout << driver.toStdString() << " ";
    }
    std::cout << std::endl;
    
    if (!drivers.contains("QMYSQL")) {
        std::cout << "❌ QMYSQL驱动不可用" << std::endl;
        return 1;
    }
    
    std::cout << "✅ QMYSQL驱动可用" << std::endl;
    
    // 尝试连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("mysql"); // 使用系统数据库
    db.setPort(3306);
    
    std::cout << "尝试连接到MySQL服务器..." << std::endl;
    
    if (!db.open()) {
        std::cout << "❌ 数据库连接失败: " << db.lastError().text().toStdString() << std::endl;
        std::cout << "错误类型: " << db.lastError().type() << std::endl;
        std::cout << "原生错误代码: " << db.lastError().nativeErrorCode().toStdString() << std::endl;
        return 1;
    }
    
    std::cout << "✅ 数据库连接成功!" << std::endl;
    
    db.close();
    return 0;
}