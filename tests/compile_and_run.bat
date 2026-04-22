@echo off
echo === Compiling Database Connection Tests ===
echo.

REM Check if Qt is available
where qmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: qmake not found in PATH
    echo Please ensure Qt is properly installed and added to PATH
    pause
    exit /b 1
)

REM Create build directory
if not exist "..\build\tests" mkdir "..\build\tests"

REM Generate Makefile
echo Generating Makefile...
qmake tests.pro -o ..\build\tests\Makefile
if %ERRORLEVEL% neq 0 (
    echo Failed to generate Makefile
    pause
    exit /b 1
)

REM Build the tests
echo Building tests...
cd ..\build\tests
make
if %ERRORLEVEL% neq 0 (
    echo Failed to build tests
    cd ..\..\tests
    pause
    exit /b 1
)

echo.
echo Build successful!
echo.
echo To run tests, execute: ..\build\tests\database_connection_tests.exe
echo.

cd ..\..\tests
pause