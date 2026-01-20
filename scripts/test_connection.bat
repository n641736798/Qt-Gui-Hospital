@echo off
echo ========================================
echo 数据库连接测试程序
echo ========================================
echo.

echo 编译测试程序...
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo 错误: 找不到 qmake 命令
    echo 请确保 Qt 环境已正确配置
    pause
    exit /b 1
)

echo QT += core sql > test.pro
echo CONFIG += console >> test.pro
echo CONFIG -= app_bundle >> test.pro
echo TARGET = test_connection >> test.pro
echo SOURCES += test_db_connection.cpp >> test.pro

qmake test.pro
if %errorlevel% neq 0 (
    echo qmake 失败
    pause
    exit /b 1
)

nmake
if %errorlevel% neq 0 (
    mingw32-make
    if %errorlevel% neq 0 (
        echo 编译失败
        pause
        exit /b 1
    )
)

echo.
echo 运行测试程序...
if exist debug\test_connection.exe (
    debug\test_connection.exe
) else if exist release\test_connection.exe (
    release\test_connection.exe
) else if exist test_connection.exe (
    test_connection.exe
) else (
    echo 找不到可执行文件
)

echo.
echo 清理临时文件...
if exist test.pro del test.pro
if exist Makefile del Makefile
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release

pause