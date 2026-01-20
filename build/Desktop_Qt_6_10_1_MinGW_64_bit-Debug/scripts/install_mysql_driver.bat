@echo off
chcp 65001 >nul
echo ========================================
echo Qt MySQL 驱动安装助手
echo ========================================
echo.

echo 1. 检测 Qt 安装位置...
echo.

REM 检查常见的 Qt 安装位置
set "QT_FOUND=0"
set "QT_PATH="

REM 检查 C:\Qt
if exist "C:\Qt" (
    echo 找到 Qt 安装目录: C:\Qt
    for /d %%i in ("C:\Qt\*") do (
        if exist "%%i\bin\qmake.exe" (
            echo   - 发现 Qt 版本: %%~ni
            set "QT_PATH=%%i"
            set "QT_FOUND=1"
        )
    )
)

REM 检查 Program Files
if exist "C:\Program Files\Qt" (
    echo 找到 Qt 安装目录: C:\Program Files\Qt
    for /d %%i in ("C:\Program Files\Qt\*") do (
        if exist "%%i\bin\qmake.exe" (
            echo   - 发现 Qt 版本: %%~ni
            set "QT_PATH=%%i"
            set "QT_FOUND=1"
        )
    )
)

REM 检查环境变量
if defined QTDIR (
    echo 找到 Qt 环境变量: %QTDIR%
    if exist "%QTDIR%\bin\qmake.exe" (
        set "QT_PATH=%QTDIR%"
        set "QT_FOUND=1"
    )
)

if "%QT_FOUND%"=="0" (
    echo.
    echo ❌ 未找到 Qt 安装
    echo.
    echo 请选择安装方式:
    echo 1. 重新安装 Qt 并包含 MySQL 驱动 ^(推荐^)
    echo 2. 手动指定 Qt 路径
    echo 3. 使用 SQLite 替代方案
    echo.
    set /p choice="请输入选择 (1-3): "
    
    if "!choice!"=="1" goto install_qt
    if "!choice!"=="2" goto manual_path
    if "!choice!"=="3" goto use_sqlite
    goto end
)

echo.
echo ✅ 找到 Qt 安装: %QT_PATH%
echo.

echo 2. 检查当前 MySQL 驱动状态...
if exist "%QT_PATH%\plugins\sqldrivers\qsqlmysql.dll" (
    echo ✅ MySQL 驱动已安装: %QT_PATH%\plugins\sqldrivers\qsqlmysql.dll
    echo.
    echo 如果仍然无法使用，可能是以下原因:
    echo - MySQL 客户端库未安装
    echo - 路径配置问题
    echo - 版本不兼容
    goto check_mysql_client
) else (
    echo ❌ MySQL 驱动未安装
    echo.
    goto install_options
)

:check_mysql_client
echo 3. 检查 MySQL 客户端库...
where mysql.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ 找到 MySQL 客户端
    mysql --version
) else (
    echo ❌ 未找到 MySQL 客户端
    echo.
    echo MySQL 驱动需要 MySQL 客户端库支持
    goto install_mysql_client
)

echo.
echo 4. 测试驱动加载...
echo 运行测试程序检查驱动状态...
call test_connection.bat
goto end

:install_options
echo 选择 MySQL 驱动安装方式:
echo.
echo 1. 下载预编译的 MySQL 驱动 ^(推荐^)
echo 2. 重新安装 Qt 并包含 MySQL 支持
echo 3. 从源码编译 MySQL 驱动
echo 4. 使用 SQLite 替代方案
echo.
set /p install_choice="请输入选择 (1-4): "

if "%install_choice%"=="1" goto download_driver
if "%install_choice%"=="2" goto install_qt
if "%install_choice%"=="3" goto compile_driver
if "%install_choice%"=="4" goto use_sqlite
goto end

:download_driver
echo.
echo 📥 下载预编译 MySQL 驱动...
echo.
echo 正在为您的 Qt 版本查找合适的驱动...

REM 检测 Qt 版本
if exist "%QT_PATH%\bin\qmake.exe" (
    "%QT_PATH%\bin\qmake.exe" -version > qt_version.tmp 2>&1
    findstr /C:"Qt version" qt_version.tmp
    del qt_version.tmp
)

echo.
echo 由于版本兼容性问题，建议使用以下方法之一:
echo.
echo 方法 1: 重新安装 Qt ^(最可靠^)
echo - 下载 Qt 在线安装器
echo - 安装时选择 "Additional Libraries" → "Qt SQL Drivers"
echo.
echo 方法 2: 使用 SQLite ^(最简单^)
echo - 无需额外配置
echo - 运行: run_with_sqlite.bat
echo.
set /p final_choice="选择 1 重新安装 Qt, 2 使用 SQLite: "

if "%final_choice%"=="1" goto install_qt
if "%final_choice%"=="2" goto use_sqlite
goto end

:install_qt
echo.
echo 🔄 重新安装 Qt 指南
echo.
echo 1. 下载 Qt 在线安装器:
echo    https://www.qt.io/download-qt-installer
echo.
echo 2. 运行安装器，选择组件时:
echo    ✅ 选择你需要的 Qt 版本 ^(如 Qt 6.5.0^)
echo    ✅ 展开 "Additional Libraries"
echo    ✅ 勾选 "Qt SQL Drivers" 或 "MySQL Driver"
echo    ✅ 选择合适的编译器 ^(MinGW 或 MSVC^)
echo.
echo 3. 安装完成后运行:
echo    test_connection.bat
echo.
echo 正在打开下载页面...
start https://www.qt.io/download-qt-installer
goto end

:install_mysql_client
echo.
echo 📦 安装 MySQL 客户端库
echo.
echo MySQL 驱动需要 MySQL 客户端库支持，请选择:
echo.
echo 1. 安装 MySQL Server ^(包含客户端库^)
echo 2. 安装 MySQL Connector/C ^(仅客户端库^)
echo 3. 使用 SQLite 替代方案
echo.
set /p mysql_choice="请输入选择 (1-3): "

if "%mysql_choice%"=="1" (
    echo 正在打开 MySQL Server 下载页面...
    start https://dev.mysql.com/downloads/mysql/
)
if "%mysql_choice%"=="2" (
    echo 正在打开 MySQL Connector/C 下载页面...
    start https://dev.mysql.com/downloads/connector/c/
)
if "%mysql_choice%"=="3" goto use_sqlite
goto end

:compile_driver
echo.
echo 🔨 从源码编译 MySQL 驱动
echo.
echo 这需要:
echo - Qt 源码
echo - MySQL 开发库
echo - 合适的编译器
echo.
echo 由于复杂性，建议选择其他方案
echo.
pause
goto install_options

:manual_path
echo.
echo 📁 手动指定 Qt 路径
echo.
set /p manual_qt_path="请输入 Qt 安装路径 (如 C:\Qt\6.5.0\mingw_64): "
if exist "%manual_qt_path%\bin\qmake.exe" (
    set "QT_PATH=%manual_qt_path%"
    echo ✅ 找到 Qt: %QT_PATH%
    goto check_mysql_client
) else (
    echo ❌ 无效的 Qt 路径
    goto manual_path
)

:use_sqlite
echo.
echo 🗃️ 使用 SQLite 数据库
echo.
echo SQLite 的优势:
echo ✅ 无需安装额外软件
echo ✅ 无需配置数据库服务
echo ✅ Qt 默认支持
echo ✅ 完全兼容项目功能
echo.
echo 立即测试 SQLite 连接:
call test_connection.bat
echo.
echo 运行程序:
echo run_with_sqlite.bat
goto end

:end
echo.
echo ========================================
echo 安装助手完成
echo ========================================
echo.
echo 如需帮助，请查看:
echo - MySQL驱动安装指南.md
echo - 数据库驱动问题解决方案.md
echo.
pause