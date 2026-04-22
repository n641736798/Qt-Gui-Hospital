@echo off
echo === Database Connection Unit Tests ===
echo.

REM Create build directory if it doesn't exist
if not exist "..\build\tests" mkdir "..\build\tests"

REM Build the tests
echo Building tests...
qmake tests.pro
if %ERRORLEVEL% neq 0 (
    echo Failed to generate Makefile
    pause
    exit /b 1
)

make
if %ERRORLEVEL% neq 0 (
    echo Failed to build tests
    pause
    exit /b 1
)

echo.
echo Running tests...
echo.

REM Run the tests
..\build\tests\database_connection_tests.exe

echo.
echo Tests completed.
pause