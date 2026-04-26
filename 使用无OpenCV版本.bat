@echo off
chcp 65001
cls
echo ==========================================
echo  医院管理系统 - 无 OpenCV 版本编译
echo ==========================================
echo.
echo 此版本禁用医学影像查看功能
echo.

:: 设置环境变量
set PATH=D:\software\Qt\Tools\mingw1310_64\bin;D:\software\Qt\6.10.1\mingw_64\bin;%PATH%

cd /d "%~dp0"

echo 正在生成 Makefile (无 OpenCV 版本)...
qmake Hospital_reorganized_no_opencv.pro

echo 正在编译...
mingw32-make -j4

echo.
echo 编译完成！
echo 可执行文件: release/Hospital_reorganized.exe
echo.
pause
