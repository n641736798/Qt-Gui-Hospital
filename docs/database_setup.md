# 数据库配置说明

## MySQL 数据库设置

### 1. 安装 MySQL
确保已安装 MySQL 服务器并正在运行。

### 2. 创建数据库和用户
```sql
-- 连接到 MySQL (使用 root 用户)
mysql -u root -p

-- 创建数据库
CREATE DATABASE hospital_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- 创建用户并授权 (可选，也可以直接使用 root)
CREATE USER 'hospital_user'@'localhost' IDENTIFIED BY 'hospital_password';
GRANT ALL PRIVILEGES ON hospital_db.* TO 'hospital_user'@'localhost';
FLUSH PRIVILEGES;
```

### 3. 当前应用程序配置

在 `src/mainwindow.cpp` 中的默认配置：
```cpp
QString host = "localhost";        // MySQL 服务器地址
QString database = "hospital_db";  // 数据库名称
QString username = "root";         // 用户名
QString password = "123456";       // 密码 (请修改为你的 MySQL root 密码)
int port = 3306;                   // MySQL 端口
```

### 4. 修改数据库连接

如果你的 MySQL 配置不同，请修改 `src/mainwindow.cpp` 中的连接参数：

```cpp
void MainWindow::setupDatabase()
{
    // 修改这些参数以匹配你的 MySQL 配置
    QString host = "localhost";           // 你的 MySQL 主机
    QString database = "hospital_db";     // 数据库名
    QString username = "your_username";   // 你的 MySQL 用户名
    QString password = "your_password";   // 你的 MySQL 密码
    
    // ... 其余代码保持不变
}
```

### 5. 数据库表结构

应用程序会自动创建以下表：

#### patients 表 (患者信息)
- `id` - 主键，自增
- `name` - 患者姓名
- `phone` - 电话号码
- `email` - 邮箱地址
- `address` - 地址
- `birth_date` - 出生日期
- `gender` - 性别
- `medical_history` - 病史
- `created_at` - 创建时间
- `updated_at` - 更新时间

#### medical_records 表 (病历记录)
- `id` - 主键，自增
- `patient_id` - 患者ID (外键)
- `created_date` - 创建日期
- `diagnosis` - 诊断信息
- `symptoms` - 症状描述
- `treatment` - 治疗方案
- `doctor_name` - 医生姓名
- `notes` - 备注

### 6. 故障排除

如果遇到连接问题：

1. **检查 MySQL 服务是否运行**
   ```bash
   # Windows
   net start mysql
   
   # Linux/Mac
   sudo systemctl start mysql
   ```

2. **验证连接参数**
   ```bash
   mysql -h localhost -u root -p
   ```

3. **检查防火墙设置**
   确保端口 3306 未被阻止

4. **查看应用程序日志**
   应用程序会在控制台输出详细的错误信息