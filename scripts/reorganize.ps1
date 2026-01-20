# 项目重组脚本
Write-Host "开始重组项目目录结构..." -ForegroundColor Green

# 创建新目录
$directories = @("src", "include", "ui", "resources", "docs", "scripts")
foreach ($dir in $directories) {
    if (!(Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force
        Write-Host "创建目录: $dir" -ForegroundColor Yellow
    }
}

# 移动源文件 (.cpp)
$sourceFiles = @("main.cpp", "mainwindow.cpp", "databasemanager.cpp", "patientdialog.cpp", "force_background_test.cpp", "test_db_connection.cpp")
foreach ($file in $sourceFiles) {
    if (Test-Path $file) {
        Move-Item $file "src/" -Force
        Write-Host "移动源文件: $file -> src/" -ForegroundColor Cyan
    }
}

# 移动头文件 (.h)
$headerFiles = @("mainwindow.h", "databasemanager.h", "patientdialog.h")
foreach ($file in $headerFiles) {
    if (Test-Path $file) {
        Move-Item $file "include/" -Force
        Write-Host "移动头文件: $file -> include/" -ForegroundColor Cyan
    }
}

# 移动UI文件
if (Test-Path "mainwindow.ui") {
    Move-Item "mainwindow.ui" "ui/" -Force
    Write-Host "移动UI文件: mainwindow.ui -> ui/" -ForegroundColor Cyan
}

# 移动资源文件
if (Test-Path "resources.qrc") {
    Move-Item "resources.qrc" "resources/" -Force
    Write-Host "移动资源文件: resources.qrc -> resources/" -ForegroundColor Cyan
}

if (Test-Path "img") {
    Move-Item "img" "resources/" -Force
    Write-Host "移动图片目录: img -> resources/" -ForegroundColor Cyan
}

# 移动文档文件
$docFiles = Get-ChildItem -Filter "*.md"
foreach ($file in $docFiles) {
    Move-Item $file.Name "docs/" -Force
    Write-Host "移动文档: $($file.Name) -> docs/" -ForegroundColor Cyan
}

if (Test-Path "LICENSE") {
    Move-Item "LICENSE" "docs/" -Force
    Write-Host "移动许可证: LICENSE -> docs/" -ForegroundColor Cyan
}

# 移动脚本文件
$scriptFiles = Get-ChildItem -Filter "*.bat"
foreach ($file in $scriptFiles) {
    Move-Item $file.Name "scripts/" -Force
    Write-Host "移动脚本: $($file.Name) -> scripts/" -ForegroundColor Cyan
}

$psFiles = Get-ChildItem -Filter "*.ps1" | Where-Object { $_.Name -ne "reorganize.ps1" }
foreach ($file in $psFiles) {
    Move-Item $file.Name "scripts/" -Force
    Write-Host "移动PowerShell脚本: $($file.Name) -> scripts/" -ForegroundColor Cyan
}

if (Test-Path "database_setup.sql") {
    Move-Item "database_setup.sql" "scripts/" -Force
    Write-Host "移动SQL脚本: database_setup.sql -> scripts/" -ForegroundColor Cyan
}

Write-Host "`n项目重组完成！" -ForegroundColor Green
Write-Host "新的目录结构：" -ForegroundColor Yellow
Write-Host "  src/        - 源代码文件 (.cpp)" -ForegroundColor White
Write-Host "  include/    - 头文件 (.h)" -ForegroundColor White
Write-Host "  ui/         - 界面文件 (.ui)" -ForegroundColor White
Write-Host "  resources/  - 资源文件 (.qrc, img/)" -ForegroundColor White
Write-Host "  docs/       - 文档文件 (.md, LICENSE)" -ForegroundColor White
Write-Host "  scripts/    - 脚本文件 (.bat, .ps1, .sql)" -ForegroundColor White
Write-Host "  build/      - 构建输出目录" -ForegroundColor White

Write-Host "`n请使用 Hospital_reorganized.pro 作为新的项目文件!" -ForegroundColor Red