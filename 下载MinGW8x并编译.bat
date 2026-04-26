@echo off
chcp 65001
cls
echo ==========================================
echo  医院管理系统 - MinGW 8.x 编译脚本
echo ==========================================
echo.
echo 步骤1: 下载 MinGW 8.1.0
echo 请从以下链接下载 MinGW 8.1.0:
echo https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z/download
echo.
echo 或者使用国内镜像:
echo https://mirrors.tuna.tsinghua.edu.cn/qt/development_releases/prebuilt/mingw_64/mingw81_64.7z
echo.
echo 步骤2: 解压到 D:\software\mingw81_64
echo.
echo 步骤3: 设置环境变量并编译
echo.
pause

:: 设置环境变量
set PATH=D:\software\mingw81_64\bin;D:\software\Qt\6.10.1\mingw_64\bin;%PATH%

cd /d "%~dp0"

echo 正在清理旧编译文件...
mingw32-make clean

echo 正在生成 Makefile...
qmake Hospital_reorganized.pro

echo 正在编译...
mingw32-make -j4

echo.
echo 编译完成！
pause
