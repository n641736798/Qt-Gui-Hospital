@echo off
chcp 65001 >nul
setlocal
set "SRC=D:\software\Qt\additional part\qcustomplot-source"
set "PROJECT=%~dp0"
set "INC=%PROJECT%include"
set "SRCDIR=%PROJECT%src"

if not exist "%SRC%\qcustomplot.h" (
    echo [ERROR] 未找到: %SRC%\qcustomplot.h
    echo 请确认 QCustomPlot 源路径正确，或修改本脚本中的 SRC 变量。
    exit /b 1
)
if not exist "%SRC%\qcustomplot.cpp" (
    echo [ERROR] 未找到: %SRC%\qcustomplot.cpp
    exit /b 1
)

copy /Y "%SRC%\qcustomplot.h" "%INC%\qcustomplot.h"
if errorlevel 1 (echo [ERROR] 复制 qcustomplot.h 失败 & exit /b 1)
echo [OK] %INC%\qcustomplot.h

copy /Y "%SRC%\qcustomplot.cpp" "%SRCDIR%\qcustomplot.cpp"
if errorlevel 1 (echo [ERROR] 复制 qcustomplot.cpp 失败 & exit /b 1)
echo [OK] %SRCDIR%\qcustomplot.cpp

echo.
echo QCustomPlot 已拷贝到项目 include/ 与 src/，可直接编译。
endlocal
