@echo off
echo ========================================
echo 病历管理功能 - 完整测试套件
echo ========================================
echo.

REM Check if MySQL is running
echo 检查MySQL服务状态...
sc query mysql >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [警告] MySQL服务未运行或未安装
    echo 请启动MySQL服务: net start mysql
    echo.
    set /p continue="是否继续运行测试? (y/n): "
    if /i not "%continue%"=="y" (
        echo 测试已取消
        pause
        exit /b 1
    )
)

echo.
echo ========================================
echo 1. 编译和运行数据库连接测试
echo ========================================
echo.

cd /d "%~dp0"

REM Test 1: Database Connection Tests
echo 编译数据库连接测试...
qmake tests.pro -o Makefile.db_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 无法生成Makefile
    goto :error
)

make -f Makefile.db_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 编译失败
    goto :error
)

echo.
echo 运行数据库连接测试...
echo.
..\build\tests\database_connection_tests.exe -o test_results_db.txt,txt
set DB_TEST_RESULT=%ERRORLEVEL%

echo.
echo ========================================
echo 2. 编译和运行病历模型测试
echo ========================================
echo.

echo 编译病历模型测试...
qmake test_medical_record_model.pro -o Makefile.model_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 无法生成Makefile
    goto :error
)

make -f Makefile.model_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 编译失败
    goto :error
)

echo.
echo 运行病历模型测试...
echo.
..\build\tests\medical_record_model_tests.exe -o test_results_model.txt,txt
set MODEL_TEST_RESULT=%ERRORLEVEL%

echo.
echo ========================================
echo 3. 编译和运行界面测试
echo ========================================
echo.

echo 编译界面测试...
qmake test_medical_record_widget.pro -o Makefile.widget_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 无法生成Makefile
    goto :error
)

make -f Makefile.widget_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 编译失败
    goto :error
)

echo.
echo 运行界面测试...
echo.
..\build\tests\medical_record_widget_tests.exe -o test_results_widget.txt,txt
set WIDGET_TEST_RESULT=%ERRORLEVEL%

echo.
echo ========================================
echo 4. 编译和运行对话框测试
echo ========================================
echo.

echo 编译对话框测试...
qmake test_medical_record_dialog.pro -o Makefile.dialog_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 无法生成Makefile
    goto :error
)

make -f Makefile.dialog_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 编译失败
    goto :error
)

echo.
echo 运行对话框测试...
echo.
..\build\tests\medical_record_dialog_tests.exe -o test_results_dialog.txt,txt
set DIALOG_TEST_RESULT=%ERRORLEVEL%

echo.
echo ========================================
echo 5. 编译和运行级联删除测试
echo ========================================
echo.

echo 编译级联删除测试...
qmake test_cascade_delete.pro -o Makefile.cascade_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 无法生成Makefile
    goto :error
)

make -f Makefile.cascade_test
if %ERRORLEVEL% neq 0 (
    echo [错误] 编译失败
    goto :error
)

echo.
echo 运行级联删除测试...
echo.
..\build\tests\cascade_delete_tests.exe -o test_results_cascade.txt,txt
set CASCADE_TEST_RESULT=%ERRORLEVEL%

echo.
echo ========================================
echo 测试结果汇总
echo ========================================
echo.

echo 1. 数据库连接测试: 
if %DB_TEST_RESULT% equ 0 (
    echo    [通过] ✓
) else (
    echo    [失败] ✗
)

echo 2. 病历模型测试: 
if %MODEL_TEST_RESULT% equ 0 (
    echo    [通过] ✓
) else (
    echo    [失败] ✗
)

echo 3. 界面测试: 
if %WIDGET_TEST_RESULT% equ 0 (
    echo    [通过] ✓
) else (
    echo    [失败] ✗
)

echo 4. 对话框测试: 
if %DIALOG_TEST_RESULT% equ 0 (
    echo    [通过] ✓
) else (
    echo    [失败] ✗
)

echo 5. 级联删除测试: 
if %CASCADE_TEST_RESULT% equ 0 (
    echo    [通过] ✓
) else (
    echo    [失败] ✗
)

echo.
echo 详细测试结果已保存到:
echo - test_results_db.txt
echo - test_results_model.txt
echo - test_results_widget.txt
echo - test_results_dialog.txt
echo - test_results_cascade.txt
echo.

REM Calculate total result
set /a TOTAL_RESULT=%DB_TEST_RESULT%+%MODEL_TEST_RESULT%+%WIDGET_TEST_RESULT%+%DIALOG_TEST_RESULT%+%CASCADE_TEST_RESULT%

if %TOTAL_RESULT% equ 0 (
    echo ========================================
    echo 所有测试通过! ✓✓✓
    echo ========================================
) else (
    echo ========================================
    echo 部分测试失败，请查看详细结果
    echo ========================================
)

echo.
pause
exit /b %TOTAL_RESULT%

:error
echo.
echo ========================================
echo 测试执行失败
echo ========================================
echo.
pause
exit /b 1
