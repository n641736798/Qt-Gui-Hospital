@echo off
echo 正在重组项目目录结构...

REM 创建新的目录结构
mkdir src 2>nul
mkdir include 2>nul
mkdir ui 2>nul
mkdir resources 2>nul
mkdir docs 2>nul
mkdir scripts 2>nul

REM 移动源文件 (.cpp)
if exist main.cpp move main.cpp src\
if exist mainwindow.cpp move mainwindow.cpp src\
if exist databasemanager.cpp move databasemanager.cpp src\
if exist patientdialog.cpp move patientdialog.cpp src\
if exist force_background_test.cpp move force_background_test.cpp src\
if exist test_db_connection.cpp move test_db_connection.cpp src\

REM 移动头文件 (.h)
if exist mainwindow.h move mainwindow.h include\
if exist databasemanager.h move databasemanager.h include\
if exist patientdialog.h move patientdialog.h include\

REM 移动UI文件
if exist mainwindow.ui move mainwindow.ui ui\

REM 移动资源文件
if exist resources.qrc move resources.qrc resources\
if exist img move img resources\

REM 移动文档文件
if exist *.md move *.md docs\
if exist LICENSE move LICENSE docs\

REM 移动脚本文件
if exist *.bat move *.bat scripts\
if exist *.ps1 move *.ps1 scripts\
if exist database_setup.sql move database_setup.sql scripts\

echo 项目重组完成！
echo.
echo 新的目录结构：
echo   src/        - 源代码文件 (.cpp)
echo   include/    - 头文件 (.h)
echo   ui/         - 界面文件 (.ui)
echo   resources/  - 资源文件 (.qrc, img/)
echo   docs/       - 文档文件 (.md, LICENSE)
echo   scripts/    - 脚本文件 (.bat, .ps1, .sql)
echo   build/      - 构建输出目录
echo.
pause