# MySQL 驱动快速安装指南

## 🎯 问题诊断

你遇到的错误：
```
QSqlDatabase: can not load requested driver 'QMYSQL'
available drivers: QIBASE QSQLITE QMIMER QOCI QODBC QPSQL
```

**原因：** Qt 安装时没有包含 MySQL 驱动

## 🚀 立即解决

### 方案 1: 使用 SQLite (推荐，0 配置)

```cmd
run_with_sqlite.bat
```

**优势：**
- ✅ 立即可用，无需任何配置
- ✅ 功能完全相同
- ✅ 数据存储在本地文件
- ✅ 适合学习和开发

### 方案 2: 安装 MySQL 驱动

#### 步骤 1: 找到你的 Qt 安装目录

运行检测工具：
```cmd
find_qt.bat
```

或手动查找常见位置：
- `C:\Qt\6.5.0\mingw_64`
- `C:\Qt\6.4.0\msvc2019_64`
- `C:\Program Files\Qt\...`

#### 步骤 2: 下载对应的 MySQL 驱动

根据你的 Qt 版本选择下载链接：

**Qt 6.5.x:**
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.5/qsqlmysql.dll

**Qt 6.4.x:**
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.4/qsqlmysql.dll

**Qt 6.3.x:**
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.3/qsqlmysql.dll

**Qt 5.15.x:**
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt5.15/qsqlmysql.dll

#### 步骤 3: 安装驱动

1. **下载 `qsqlmysql.dll` 文件**
2. **复制到 Qt 插件目录：**
   ```
   [你的Qt目录]\plugins\sqldrivers\qsqlmysql.dll
   ```
   
   例如：
   ```
   C:\Qt\6.5.0\mingw_64\plugins\sqldrivers\qsqlmysql.dll
   ```

#### 步骤 4: 安装 MySQL 客户端

MySQL 驱动需要 MySQL 客户端库：

**选择 1: MySQL Server (完整版)**
- 下载：https://dev.mysql.com/downloads/mysql/
- 安装完整的 MySQL Server

**选择 2: MySQL Connector/C (轻量版)**
- 下载：https://dev.mysql.com/downloads/connector/c/
- 仅安装客户端库

#### 步骤 5: 测试安装

```cmd
test_connection.bat
```

## 📋 快速操作清单

### 如果你想立即使用 (推荐)
- [ ] 运行 `run_with_sqlite.bat`
- [ ] 享受完整功能的医院管理系统

### 如果你坚持使用 MySQL
- [ ] 运行 `find_qt.bat` 找到 Qt 目录
- [ ] 下载对应版本的 `qsqlmysql.dll`
- [ ] 复制到 `[Qt目录]\plugins\sqldrivers\`
- [ ] 安装 MySQL 客户端库
- [ ] 运行 `test_connection.bat` 验证

## 🔍 版本对应表

| Qt 版本 | 下载链接 |
|---------|----------|
| 6.5.x | [qsqlmysql.dll](https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.5/qsqlmysql.dll) |
| 6.4.x | [qsqlmysql.dll](https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.4/qsqlmysql.dll) |
| 6.3.x | [qsqlmysql.dll](https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.3/qsqlmysql.dll) |
| 5.15.x | [qsqlmysql.dll](https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt5.15/qsqlmysql.dll) |

## ❓ 常见问题

**Q: 我不知道我的 Qt 版本**
A: 运行 `find_qt.bat` 会显示所有信息

**Q: 下载的驱动不工作**
A: 确保版本匹配，并安装了 MySQL 客户端库

**Q: 我想要最简单的方案**
A: 使用 SQLite，运行 `run_with_sqlite.bat`

**Q: SQLite 和 MySQL 有什么区别？**
A: 对于这个项目，功能完全相同。SQLite 更简单，MySQL 更适合多用户环境。

## 🎯 推荐路径

1. **学习/开发阶段：** 使用 SQLite (`run_with_sqlite.bat`)
2. **生产环境：** 根据需要选择 MySQL 或 PostgreSQL
3. **遇到问题：** 优先选择 SQLite，稳定可靠

现在就开始使用吧！ 🚀