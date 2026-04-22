@echo off
echo === MySQL连接检查工具 ===
echo.

echo 1. 检查MySQL服务状态...
sc query mysql >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo ✅ MySQL服务已安装
    sc query mysql | find "STATE" | find "RUNNING" >nul
    if %ERRORLEVEL% equ 0 (
        echo ✅ MySQL服务正在运行
    ) else (
        echo ❌ MySQL服务未运行
        echo 尝试启动MySQL服务...
        net start mysql
    )
) else (
    echo ❌ MySQL服务未安装或未找到
    echo 请安装MySQL服务器
)

echo.
echo 2. 检查MySQL端口3306...
netstat -an | find ":3306" >nul
if %ERRORLEVEL% equ 0 (
    echo ✅ 端口3306正在监听
) else (
    echo ❌ 端口3306未在监听
)

echo.
echo 3. 尝试连接MySQL...
mysql -h localhost -u root -p123456 -e "SELECT 1;" 2>nul
if %ERRORLEVEL% equ 0 (
    echo ✅ MySQL连接成功
) else (
    echo ❌ MySQL连接失败
    echo 请检查用户名和密码
)

echo.
echo 4. 检查测试数据库...
mysql -h localhost -u root -p123456 -e "CREATE DATABASE IF NOT EXISTS hospital_test_db;" 2>nul
if %ERRORLEVEL% equ 0 (
    echo ✅ 测试数据库创建成功
) else (
    echo ❌ 无法创建测试数据库
)

echo.
echo === 检查完成 ===
pause