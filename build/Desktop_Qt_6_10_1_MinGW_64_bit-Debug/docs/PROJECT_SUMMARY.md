# Hospital Management System - 项目总结

## 项目概述

这是一个基于 **Qt/C++** 开发的医院管理系统 GUI 应用程序，采用现代化的暖色主题界面设计，提供实时心电图监护和患者管理功能。

## 技术栈

| 技术 | 版本/说明 |
|------|----------|
| 语言 | C++17 |
| 框架 | Qt (Core, GUI, Widgets, Charts) |
| 数据库 | MySQL |
| 构建系统 | qmake (.pro) |
| UI 设计 | Qt Designer (.ui) |

## 项目结构

```
Hospital/
├── Hospital.pro        # Qt 项目配置文件
├── main.cpp            # 程序入口
├── mainwindow.h        # 主窗口头文件
├── mainwindow.cpp      # 主窗口实现
├── mainwindow.ui       # UI 界面定义
├── resources.qrc       # 资源文件配置
├── databasemanager.h   # 数据库管理头文件
├── databasemanager.cpp # 数据库管理实现
├── patientdialog.h     # 患者对话框头文件
├── patientdialog.cpp   # 患者对话框实现
├── database_setup.sql  # 数据库初始化脚本
└── img/                # 图标资源目录
    ├── cursor.png      # 自定义光标
    ├── health.png      # 医院图标
    ├── Dashboard.png   # 仪表盘图标
    ├── Patients.png    # 患者图标
    ├── Appointments.png# 预约图标
    ├── Settings.png    # 设置图标
    ├── maximize.svg    # 最大化按钮
    ├── minus.svg       # 最小化按钮
    └── x.svg           # 关闭按钮
```

## 功能模块

### 1. 心电图监护仪表盘 (ECG Dashboard)
- **4通道实时心电图** - 显示导联 I、II、III、aVR 的实时心电波形
- **生理参数监测** - 显示心率 (72 BPM)、血压 (120/80 mmHg)、血氧饱和度 (98% SpO2)
- **监护控制** - 开始/暂停心电图实时监测
- **波形特征** - 模拟真实的 P 波、QRS 复合波、T 波心电信号

### 2. 患者管理 (Patients)
- **完整的患者信息管理** - 姓名、电话、邮箱、出生日期、性别、病史
- **数据库集成** - MySQL 数据库存储患者信息
- **搜索功能** - 按姓名、电话或邮箱搜索患者
- **CRUD 操作** - 添加、编辑、删除患者信息
- **表格视图** - 直观的患者信息列表显示

### 3. 预约管理 (Appointments)
- **日历控件** - 可视化日期选择
- **预约列表** - 表格显示预约信息
- **新建预约** - 添加新的预约记录

## 数据库配置

### 连接参数
- **主机**: localhost
- **用户名**: root  
- **密码**: 123456
- **数据库**: hospital_db

### 自动数据库创建
系统会自动检测数据库连接，如果 `hospital_db` 不存在会提示用户创建。

## UI 特性

### 界面设计
- **暖色主题** - 采用 #F5E6D3、#E8D5C4、#FF8C42 等暖色调
- **无边框窗口** - 自定义标题栏，支持拖拽移动
- **可折叠侧边栏** - 动画效果展开/收起（60px ↔ 200px）
- **自定义光标** - 16x16 像素自定义鼠标指针

### 心电图特性
- **实时更新** - 每50ms刷新一次，模拟真实心电监护仪
- **滚动显示** - 心电图波形从右向左滚动显示
- **多通道同步** - 4个导联同时显示，每个通道略有相位差
- **医学级波形** - 包含完整的心电图特征波形

## 编译运行

### 环境要求
- Qt 5.12+ (包含 Charts 模块)
- MySQL 5.7+
- C++17 编译器

### 构建步骤
```bash
# 使用 qmake 构建
qmake Hospital.pro
make

# 或使用 Qt Creator 直接打开 Hospital.pro
```

### 数据库设置
1. 确保 MySQL 服务运行
2. 使用 root/123456 或修改代码中的连接参数
3. 首次运行时选择创建数据库

## 作者信息

- **作者**: Wentuma
- **YouTube**: [@Wentuma](https://www.youtube.com/@Wentuma)
- **GitHub**: [wwentuma-ship-it](https://github.com/wwentuma-ship-it)

## 许可证

项目包含 LICENSE 文件，具体许可证类型请查看该文件。
