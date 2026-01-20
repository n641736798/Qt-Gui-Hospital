@echo off
chcp 65001 >nul
echo ========================================
echo Qt 安装检测工具
echo ========================================
echo.

echo 正在搜索 Qt 安装...
echo.

REM 检查常见位置
set "FOUND_QT=0"

echo 检查 C:\Qt...
if exist "C:\Qt" (
    for /d %%i in ("C:\Qt\*") do (
        if exist "%%i\bin\qmake.exe" (
            echo   找到: %%i
            echo   版本: %%~ni
            
            REM 检查 MySQL 驱动
            if exist "%%i\plugins\sqldrivers\qsqlmysql.dll" (
                echo   MySQL 驱动: ✅ 已安装
            ) else (
                echo   MySQL 驱动: ❌ 未安装
            )
            echo.
            set "FOUND_QT=1"
        )
    )
)

echo 检查 Program Files...
if exist "C:\Program Files\Qt" (
    for /d %%i in ("C:\Program Files\Qt\*") do (
        if exist "%%i\bin\qmake.exe" (
            echo   找到: %%i
            echo   版本: %%~ni
            
            REM 检查 MySQL 驱动
            if exist "%%i\plugins\sqldrivers\qsqlmysql.dll" (
                echo   MySQL 驱动: ✅ 已安装
            ) else (
                echo   MySQL 驱动: ❌ 未安装
            )
            echo.
            set "FOUND_QT=1"
        )
    )
)

echo 检查环境变量...
if defined QTDIR (
    echo   QTDIR = %QTDIR%
    if exist "%QTDIR%\bin\qmake.exe" (
        echo   状态: ✅ 有效
        if exist "%QTDIR%\plugins\sqldrivers\qsqlmysql.dll" (
            echo   MySQL 驱动: ✅ 已安装
        ) else (
            echo   MySQL 驱动: ❌ 未安装
        )
        set "FOUND_QT=1"
    ) else (
        echo   状态: ❌ 无效路径
    )
    echo.
) else (
    echo   QTDIR 环境变量未设置
    echo.
)

if "%FOUND_QT%"=="0" (
    echo ❌ 未找到 Qt 安装
    echo.
    echo 可能的解决方案:
    echo 1. 安装 Qt: https://www.qt.io/download-qt-installer
    echo 2. 使用 SQLite 替代: run_with_sqlite.bat
    echo.
) else (
    echo ✅ 找到 Qt 安装
    echo.
    echo 下一步:
    echo 1. 如果 MySQL 驱动未安装，请手动下载安装
    echo 2. 或者使用 SQLite: run_with_sqlite.bat
    echo.
)

echo 检查可用的数据库驱动...
echo 运行测试程序...
echo.
call test_connection.bat

pause