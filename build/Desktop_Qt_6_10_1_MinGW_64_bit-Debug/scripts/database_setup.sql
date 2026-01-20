-- 医院管理系统数据库设置脚本
-- 请在 MySQL 中执行此脚本

-- 1. 创建数据库
CREATE DATABASE IF NOT EXISTS hospital_db;

-- 2. 使用数据库
USE hospital_db;

-- 3. 创建患者表
CREATE TABLE IF NOT EXISTS patients (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(20),
    email VARCHAR(100),
    address TEXT,
    birth_date DATE,
    gender ENUM('Male', 'Female', 'Other') DEFAULT 'Male',
    medical_history TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- 4. 插入示例数据
INSERT INTO patients (name, phone, email, address, birth_date, gender, medical_history) VALUES
('张三', '138-0000-0001', 'zhangsan@email.com', '北京市朝阳区建国路1号', '1985-03-15', 'Male', '高血压'),
('李四', '138-0000-0002', 'lisi@email.com', '上海市浦东新区陆家嘴路2号', '1990-07-22', 'Female', '糖尿病'),
('王五', '138-0000-0003', 'wangwu@email.com', '广州市天河区珠江路3号', '1978-11-08', 'Male', '哮喘'),
('赵六', '138-0000-0004', 'zhaoliu@email.com', '深圳市南山区科技路4号', '1995-01-30', 'Female', '无已知过敏'),
('钱七', '138-0000-0005', 'qianqi@email.com', '杭州市西湖区文三路5号', '1982-09-12', 'Male', '心脏病');

-- 5. 显示创建结果
SELECT 'Database and tables created successfully!' as Status;
SELECT COUNT(*) as 'Total Patients' FROM patients;