# Qt MySQL 驱动手动安装指南

## 🚀 快速安装

### 方法 1: 自动安装脚本 (推荐)
```cmd
auto_install_mysql.bat
```

### 方法 2: 交互式安装助手
```cmd
install_mysql_driver.bat
```

## 📋 手动安装步骤

### 步骤 1: 找到 Qt 安装目录

常见位置：
- `C:\Qt\6.5.0\mingw_64`
- `C:\Qt\6.4.0\msvc2019_64`
- `C:\Program Files\Qt\Qt6.5.0\6.5.0\mingw_64`

### 步骤 2: 检查当前驱动

查看目录：`[Qt安装目录]\plugins\sqldrivers\`

应该包含：
- `qsqlite.dll` ✅ (通常存在)
- `qsqlmysql.dll` ❌ (需要安装)

### 步骤 3: 下载 MySQL 驱动

根据你的 Qt 版本选择：

#### Qt 6.5.x
```
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.5/qsqlmysql.dll
```

#### Qt 6.4.x
```
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.4/qsqlmysql.dll
```

#### Qt 6.3.x
```
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.3/qsqlmysql.dll
```

#### Qt 5.15.x
```
https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt5.15/qsqlmysql.dll
```

### 步骤 4: 安装驱动

1. **下载对应版本的 `qsqlmysql.dll`**
2. **复制到 Qt 插件目录**：
   ```
   [Qt安装目录]\plugins\sqldrivers\qsqlmysql.dll
   ```
3. **确保文件权限正确**

### 步骤 5: 安装 MySQL 客户端库

MySQL 驱动需要 MySQL 客户端库支持：

#### 选项 1: MySQL Server (完整安装)
- 下载：https://dev.mysql.com/downloads/mysql/
- 安装完整的 MySQL Server
- 包含所有必需的客户端库

#### 选项 2: MySQL Connector/C (仅客户端)
- 下载：https://dev.mysql.com/downloads/connector/c/
- 仅安装客户端库，体积更小
- 适合只需要连接功能的场景

### 步骤 6: 验证安装

运行测试程序：
```cmd
test_connection.bat
```

预期输出：
```
✅ 选择 MySQL 驱动
✅ 数据库连接成功!
✅ MySQL 数据库测试通过！
```

## 🔧 故障排除

### 问题 1: "Driver not loaded"

**可能原因：**
- MySQL 客户端库未安装
- 驱动版本不匹配
- 文件权限问题

**解决方案：**
1. 安装 MySQL 客户端库
2. 检查驱动版本是否匹配 Qt 版本
3. 以管理员身份运行安装

### 问题 2: "Access denied"

**解决方案：**
1. 检查 MySQL 服务是否运行
2. 验证用户名密码 (root/123456)
3. 确保数据库存在

### 问题 3: 找不到 Qt 安装目录

**查找方法：**
1. 检查开始菜单中的 Qt 快捷方式
2. 查看环境变量 `QTDIR`
3. 搜索 `qmake.exe` 文件位置

## 📁 目录结构示例

正确安装后的目录结构：
```
C:\Qt\6.5.0\mingw_64\
├── bin\
│   ├── qmake.exe
│   └── ...
├── plugins\
│   └── sqldrivers\
│       ├── qsqlite.dll    ✅
│       ├── qsqlmysql.dll  ✅ (新安装)
│       └── ...
└── ...
```

## 🎯 验证清单

安装完成后检查：

- [ ] `qsqlmysql.dll` 存在于 `plugins\sqldrivers\` 目录
- [ ] MySQL 客户端库已安装
- [ ] MySQL 服务正在运行 (如果使用本地 MySQL)
- [ ] 测试程序运行成功
- [ ] 应用程序可以连接数据库

## 🔄 替代方案

如果 MySQL 驱动安装困难：

### 使用 SQLite (推荐)
```cmd
run_with_sqlite.bat
```

**优势：**
- 无需额外配置
- Qt 默认支持
- 功能完全兼容
- 适合单机使用

### 重新安装 Qt
1. 下载 Qt 在线安装器
2. 安装时选择 "Additional Libraries"
3. 勾选 "Qt SQL Drivers"

## 📞 获取帮助

如果仍有问题：
1. 运行 `install_mysql_driver.bat` 获取详细诊断
2. 查看 `数据库驱动问题解决方案.md`
3. 考虑使用 SQLite 替代方案