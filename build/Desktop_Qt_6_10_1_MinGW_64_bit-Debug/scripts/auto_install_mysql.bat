@echo off
chcp 65001 >nul
echo ========================================
echo Qt MySQL 驱动自动安装
echo ========================================
echo.

echo 正在启动自动安装脚本...
echo.

REM 检查 PowerShell 执行策略
powershell -Command "Get-ExecutionPolicy" | findstr /C:"Restricted" >nul
if %errorlevel% equ 0 (
    echo 检测到 PowerShell 执行策略限制，正在临时调整...
    powershell -Command "Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process -Force; .\download_mysql_driver.ps1"
) else (
    powershell -ExecutionPolicy Bypass -File "download_mysql_driver.ps1"
)

echo.
echo 安装完成！按任意键继续...
pause >nul