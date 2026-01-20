@echo off
echo ========================================
echo MySQL 数据库快速设置脚本
echo ========================================
echo.

echo 你的数据库信息:
echo 主机: localhost
echo 用户名: root
echo 密码: 123456
echo 数据库名: hospital_db
echo.

echo 1. 检查 MySQL 服务状态...
sc query MySQL80 | find "RUNNING" >nul
if %errorlevel% equ 0 (
    echo ✓ MySQL 服务正在运行
) else (
    echo ✗ MySQL 服务未运行，尝试启动...
    net start MySQL80
    if %errorlevel% neq 0 (
        echo 尝试启动其他可能的 MySQL 服务名...
        net start MySQL
        if %errorlevel% neq 0 (
            net start MySQL57
            if %errorlevel% neq 0 (
                echo 错误: 无法启动 MySQL 服务
                echo 请手动启动 MySQL 服务或检查安装
                pause
                exit /b 1
            )
        )
    )
    echo ✓ MySQL 服务已启动
)

echo.
echo 2. 测试数据库连接...
mysql -h localhost -u root -p123456 -e "SELECT 1;" >nul 2>&1
if %errorlevel% neq 0 (
    echo ✗ 数据库连接失败
    echo 可能的原因:
    echo - 密码不正确
    echo - MySQL 未正确安装
    echo - 端口被占用
    echo.
    echo 请手动测试连接: mysql -h localhost -u root -p123456
    pause
    exit /b 1
) else (
    echo ✓ 数据库连接成功
)

echo.
echo 3. 创建数据库和表...
mysql -h localhost -u root -p123456 < database_setup.sql
if %errorlevel% neq 0 (
    echo ✗ 数据库创建失败
    echo 请检查 database_setup.sql 文件是否存在
    pause
    exit /b 1
) else (
    echo ✓ 数据库和表创建成功
)

echo.
echo 4. 验证数据库设置...
mysql -h localhost -u root -p123456 -e "USE hospital_db; SELECT COUNT(*) as 'Patient Count' FROM patients;"
if %errorlevel% neq 0 (
    echo ✗ 数据库验证失败
) else (
    echo ✓ 数据库设置完成
)

echo.
echo ========================================
echo 数据库设置完成！
echo 现在可以运行 Qt 应用程序了
echo ========================================
pause