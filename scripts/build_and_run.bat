@echo off
echo ========================================
echo 医院管理系统 - 编译和运行脚本
echo ========================================
echo.

echo 1. 检查 Qt 环境...
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo 错误: 找不到 qmake 命令
    echo 请确保：
    echo - 已安装 Qt
    echo - Qt 的 bin 目录已添加到系统 PATH
    echo - 或者在 Qt Creator 中打开项目
    echo.
    pause
    exit /b 1
)

echo 2. 清理旧的构建文件...
if exist Makefile del Makefile
if exist debug rmdir /s /q debug
if exist release rmdir /s /q release

echo 3. 生成 Makefile...
qmake Hospital.pro
if %errorlevel% neq 0 (
    echo 错误: qmake 失败
    pause
    exit /b 1
)

echo 4. 编译项目...
nmake
if %errorlevel% neq 0 (
    echo 尝试使用 mingw32-make...
    mingw32-make
    if %errorlevel% neq 0 (
        echo 错误: 编译失败
        echo 请检查编译器是否正确安装
        pause
        exit /b 1
    )
)

echo 5. 编译成功！
echo.
echo 注意: 运行程序前请确保：
echo - MySQL 服务正在运行
echo - 已创建 hospital_db 数据库
echo - 数据库连接信息正确 (localhost, root, 123456)
echo.

echo 是否现在运行程序？ (Y/N)
set /p choice=
if /i "%choice%"=="Y" (
    if exist debug\Hospital.exe (
        echo 启动程序...
        debug\Hospital.exe
    ) else if exist release\Hospital.exe (
        echo 启动程序...
        release\Hospital.exe
    ) else (
        echo 错误: 找不到可执行文件
    )
)

pause