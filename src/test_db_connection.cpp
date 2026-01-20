#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "=== 数据库连接测试程序 ===" << std::endl;
    std::cout << "数据库信息:" << std::endl;
    std::cout << "主机: localhost" << std::endl;
    std::cout << "用户名: root" << std::endl;
    std::cout << "密码: 123456" << std::endl;
    std::cout << "数据库: hospital_db" << std::endl;
    std::cout << std::endl;
    
    // 检查可用的数据库驱动
    std::cout << "1. 检查可用的数据库驱动..." << std::endl;
    QStringList drivers = QSqlDatabase::drivers();
    std::cout << "可用驱动: ";
    for (const QString &driver : drivers) {
        std::cout << driver.toStdString() << " ";
    }
    std::cout << std::endl;
    
    if (!drivers.contains("QMYSQL")) {
        std::cout << "❌ 错误: QMYSQL 驱动不可用" << std::endl;
        std::cout << "请确保 Qt 安装时包含了 MySQL 驱动" << std::endl;
        return 1;
    }
    std::cout << "✅ QMYSQL 驱动可用" << std::endl;
    std::cout << std::endl;
    
    // 尝试连接数据库
    std::cout << "2. 尝试连接数据库..." << std::endl;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("hospital_db");
    db.setPort(3306);
    
    if (!db.open()) {
        std::cout << "❌ 数据库连接失败: " << db.lastError().text().toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << "可能的解决方案:" << std::endl;
        std::cout << "1. 确保 MySQL 服务正在运行" << std::endl;
        std::cout << "2. 检查用户名和密码是否正确" << std::endl;
        std::cout << "3. 确保数据库 'hospital_db' 存在" << std::endl;
        std::cout << "4. 运行 setup_database.bat 脚本" << std::endl;
        return 1;
    }
    
    std::cout << "✅ 数据库连接成功!" << std::endl;
    std::cout << std::endl;
    
    // 检查表是否存在
    std::cout << "3. 检查数据库表..." << std::endl;
    QSqlQuery query;
    if (query.exec("SHOW TABLES LIKE 'patients'")) {
        if (query.next()) {
            std::cout << "✅ patients 表存在" << std::endl;
            
            // 检查数据
            if (query.exec("SELECT COUNT(*) FROM patients")) {
                if (query.next()) {
                    int count = query.value(0).toInt();
                    std::cout << "✅ 患者数据: " << count << " 条记录" << std::endl;
                }
            }
        } else {
            std::cout << "❌ patients 表不存在" << std::endl;
            std::cout << "请运行 setup_database.bat 创建表" << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "=== 测试完成 ===" << std::endl;
    std::cout << "如果所有检查都通过，你可以正常运行医院管理系统了！" << std::endl;
    
    db.close();
    return 0;
}