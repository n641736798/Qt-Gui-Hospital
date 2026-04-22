# MySQL 数据库设置指南

## 概述
病历管理功能的测试需要MySQL数据库服务器运行。本指南将帮助您设置和配置MySQL环境。

## 前提条件

### 检查MySQL是否已安装
打开命令提示符（CMD）并运行：
```batch
mysql --version
```

如果看到版本信息，说明MySQL已安装。如果没有，请按照下面的安装步骤操作。

## 安装MySQL（如果尚未安装）

### 方法 1: 使用MySQL Installer（推荐）
1. 访问 [MySQL官方网站](https://dev.mysql.com/downloads/installer/)
2. 下载MySQL Installer for Windows
3. 运行安装程序
4. 选择"Developer Default"或"Server only"
5. 按照安装向导完成安装
6. 在配置步骤中：
   - 设置root密码为: `123456`（或记住您设置的密码）
   - 选择默认端口: `3306`
   - 选择"Configure MySQL Server as a Windows Service"

### 方法 2: 使用Chocolatey
如果您已安装Chocolatey包管理器：
```batch
choco install mysql
```

## 启动MySQL服务

### 检查MySQL服务状态
```batch
sc query mysql
```

### 启动MySQL服务
```batch
net start mysql
```

如果遇到权限错误，请以管理员身份运行命令提示符。

### 设置MySQL服务自动启动
```batch
sc config mysql start= auto
```

## 配置MySQL用户和权限

### 连接到MySQL
```batch
mysql -u root -p
```
输入密码: `123456`（或您设置的密码）

### 创建测试数据库（可选）
测试会自动创建所需的数据库，但您也可以手动创建：
```sql
CREATE DATABASE hospital_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE DATABASE hospital_test_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE DATABASE hospital_test_widget_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE DATABASE hospital_db_test CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

### 验证权限
确保root用户有创建数据库的权限：
```sql
SHOW GRANTS FOR 'root'@'localhost';
```

应该看到类似以下的输出：
```
GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' WITH GRANT OPTION
```

### 退出MySQL
```sql
EXIT;
```

## 测试MySQL连接

### 使用命令行测试
```batch
mysql -h localhost -P 3306 -u root -p123456 -e "SELECT 1;"
```

如果成功，您应该看到：
```
+---+
| 1 |
+---+
| 1 |
+---+
```

## 配置应用程序数据库连接

### 默认配置
应用程序和测试使用以下默认配置：
```
主机: localhost
端口: 3306
用户名: root
密码: 123456
数据库: hospital_db（主应用）
```

### 修改配置（如果需要）
如果您的MySQL配置不同，需要修改以下文件：

#### 主应用程序
文件: `src/mainwindow.cpp`
```cpp
void MainWindow::setupDatabase()
{
    QString host = "localhost";      // 修改为您的主机
    QString database = "hospital_db"; // 修改为您的数据库名
    QString username = "root";        // 修改为您的用户名
    QString password = "123456";      // 修改为您的密码
    // ...
}
```

#### 测试文件
需要修改每个测试文件中的TestConfig结构：

**test_database_connection.cpp**:
```cpp
struct TestConfig {
    QString validHost = "localhost";
    QString validDatabase = "hospital_test_db";
    QString validUsername = "root";
    QString validPassword = "123456";  // 修改为您的密码
    int validPort = 3306;
} config;
```

类似地修改：
- `test_medical_record_model.cpp`
- `test_medical_record_widget.cpp`
- `test_cascade_delete.cpp`

## 常见问题和解决方案

### 问题 1: "MySQL服务未运行"
**错误**: `Can't connect to MySQL server on 'localhost'`

**解决方案**:
```batch
net start mysql
```

### 问题 2: "访问被拒绝"
**错误**: `Access denied for user 'root'@'localhost'`

**解决方案**:
1. 检查密码是否正确
2. 重置root密码：
```batch
mysqladmin -u root password "123456"
```

### 问题 3: "端口已被占用"
**错误**: MySQL无法启动，端口3306被占用

**解决方案**:
1. 检查哪个程序占用了端口：
```batch
netstat -ano | findstr :3306
```
2. 停止占用端口的程序或更改MySQL端口

### 问题 4: "MySQL驱动未找到"
**错误**: `QMYSQL driver not loaded`

**解决方案**:
1. 确保Qt安装了MySQL驱动
2. 检查Qt安装目录下的plugins/sqldrivers文件夹
3. 如果缺少qsqlmysql.dll，需要重新编译Qt或下载预编译的驱动

### 问题 5: "无法创建数据库"
**错误**: `Can't create database`

**解决方案**:
确保用户有CREATE权限：
```sql
GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

## 验证设置

### 运行快速连接测试
创建一个简单的测试脚本 `test_connection.bat`:
```batch
@echo off
echo 测试MySQL连接...
mysql -h localhost -P 3306 -u root -p123456 -e "SELECT 'MySQL连接成功!' AS Status;"
if %ERRORLEVEL% equ 0 (
    echo [成功] MySQL连接正常
) else (
    echo [失败] MySQL连接失败
)
pause
```

### 运行数据库连接测试
```batch
cd tests
..\build\tests\database_connection_tests.exe
```

如果测试通过，您的MySQL环境已正确配置！

## 性能优化建议

### 对于测试环境
在 `my.ini` 或 `my.cnf` 中添加以下配置以提高测试性能：
```ini
[mysqld]
# 增加连接数
max_connections = 200

# 优化InnoDB设置
innodb_buffer_pool_size = 256M
innodb_log_file_size = 64M
innodb_flush_log_at_trx_commit = 2

# 禁用二进制日志（仅测试环境）
skip-log-bin
```

重启MySQL服务以应用更改：
```batch
net stop mysql
net start mysql
```

## 安全建议

### 生产环境
在生产环境中，不要使用默认密码！

1. 创建专用数据库用户：
```sql
CREATE USER 'hospital_user'@'localhost' IDENTIFIED BY 'strong_password_here';
GRANT ALL PRIVILEGES ON hospital_db.* TO 'hospital_user'@'localhost';
FLUSH PRIVILEGES;
```

2. 更新应用程序配置以使用新用户

### 测试环境
测试环境可以使用简单密码，但确保：
- 不要在公共网络上暴露MySQL端口
- 使用防火墙限制访问
- 定期备份测试数据

## 下一步

设置完成后，您可以：
1. 运行主应用程序测试数据库连接
2. 运行测试套件验证所有功能
3. 开始使用病历管理功能

运行所有测试：
```batch
cd tests
run_all_tests.bat
```

## 获取帮助

如果遇到问题：
1. 查看MySQL错误日志（通常在MySQL安装目录的data文件夹中）
2. 检查Windows事件查看器中的应用程序日志
3. 参考MySQL官方文档: https://dev.mysql.com/doc/

祝您测试顺利！
