# 数据库配置说明

## 前置要求

1. **安装 MySQL 服务器**
   - Windows: 下载 MySQL Installer
   - 或使用 XAMPP/WAMP 等集成环境

2. **安装 Qt MySQL 驱动**
   ```bash
   # 确保 Qt 安装时包含了 MySQL 驱动
   # 或者重新安装 Qt 并选择 MySQL 支持
   ```

## 数据库设置步骤

### 1. 创建数据库
```sql
CREATE DATABASE hospital_db;
```

### 2. 创建 MySQL 用户（可选）
```sql
CREATE USER 'hospital_user'@'localhost' IDENTIFIED BY 'your_password';
GRANT ALL PRIVILEGES ON hospital_db.* TO 'hospital_user'@'localhost';
FLUSH PRIVILEGES;
```

### 3. 修改连接配置
在 `mainwindow.cpp` 的 `setupDatabase()` 函数中修改以下参数：

```cpp
QString host = "localhost";        // MySQL 服务器地址
QString database = "hospital_db";  // 数据库名
QString username = "root";         // 用户名
QString password = "";             // 密码（设置你的 MySQL 密码）
int port = 3306;                   // 端口号
```

## 编译要求

确保 `Hospital.pro` 文件包含：
```
QT += core gui charts sql
```

## 功能特性

### 患者管理
- ✅ 添加新患者
- ✅ 编辑患者信息
- ✅ 删除患者
- ✅ 搜索患者（按姓名、电话、邮箱）
- ✅ 患者列表显示

### 数据库功能
- ✅ 自动创建表结构
- ✅ 插入示例数据
- ✅ 完整的 CRUD 操作
- ✅ 搜索功能
- ✅ 数据统计（为仪表盘准备）

### 界面特性
- ✅ 深色主题
- ✅ 响应式表格
- ✅ 模态对话框
- ✅ 实时搜索
- ✅ 数据验证

## 故障排除

### 1. "QSqlDatabase: QMYSQL driver not loaded"
- 重新安装 Qt，确保包含 MySQL 驱动
- 或下载对应的 MySQL 驱动文件

### 2. "Access denied for user"
- 检查 MySQL 用户名和密码
- 确保用户有数据库访问权限

### 3. "Can't connect to MySQL server"
- 确保 MySQL 服务正在运行
- 检查端口号和主机地址

### 4. 编译错误
- 确保 `QT += sql` 在 .pro 文件中
- 清理并重新构建项目

## 下一步扩展

可以继续添加：
- 预约管理功能
- 医生管理
- 科室管理
- 报告生成
- 数据导出
- 用户权限管理