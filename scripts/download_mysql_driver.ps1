# Qt MySQL 驱动自动下载安装脚本
param(
    [string]$QtPath = "",
    [string]$QtVersion = ""
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Qt MySQL 驱动自动安装脚本" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 函数：检测 Qt 安装
function Find-QtInstallation {
    $qtPaths = @()
    
    # 检查常见位置
    $commonPaths = @(
        "C:\Qt",
        "C:\Program Files\Qt",
        "C:\Program Files (x86)\Qt",
        $env:QTDIR
    )
    
    foreach ($basePath in $commonPaths) {
        if (Test-Path $basePath) {
            Get-ChildItem -Path $basePath -Directory -ErrorAction SilentlyContinue | ForEach-Object {
                $qmakePath = Join-Path $_.FullName "bin\qmake.exe"
                if (Test-Path $qmakePath) {
                    $qtPaths += @{
                        Path = $_.FullName
                        Version = $_.Name
                        QMake = $qmakePath
                    }
                }
            }
        }
    }
    
    return $qtPaths
}

# 函数：获取 Qt 版本信息
function Get-QtVersion {
    param([string]$qmakePath)
    
    try {
        $output = & $qmakePath -version 2>&1
        $versionLine = $output | Where-Object { $_ -match "Qt version" }
        if ($versionLine -match "Qt version (\d+\.\d+\.\d+)") {
            return $matches[1]
        }
    }
    catch {
        return "Unknown"
    }
    return "Unknown"
}

# 函数：检查 MySQL 驱动
function Test-MySQLDriver {
    param([string]$qtPath)
    
    $driverPath = Join-Path $qtPath "plugins\sqldrivers\qsqlmysql.dll"
    return Test-Path $driverPath
}

# 函数：下载文件
function Download-File {
    param(
        [string]$Url,
        [string]$OutputPath
    )
    
    try {
        Write-Host "正在下载: $Url" -ForegroundColor Yellow
        Invoke-WebRequest -Uri $Url -OutFile $OutputPath -UseBasicParsing
        return $true
    }
    catch {
        Write-Host "下载失败: $($_.Exception.Message)" -ForegroundColor Red
        return $false
    }
}

# 主程序开始
Write-Host "1. 检测 Qt 安装..." -ForegroundColor Green

$qtInstallations = Find-QtInstallation

if ($qtInstallations.Count -eq 0) {
    Write-Host "❌ 未找到 Qt 安装" -ForegroundColor Red
    Write-Host ""
    Write-Host "请先安装 Qt，推荐方法:" -ForegroundColor Yellow
    Write-Host "1. 下载 Qt 在线安装器: https://www.qt.io/download-qt-installer"
    Write-Host "2. 安装时选择 'Additional Libraries' -> 'Qt SQL Drivers'"
    Write-Host ""
    Write-Host "或者使用 SQLite 替代方案: run_with_sqlite.bat" -ForegroundColor Cyan
    exit 1
}

Write-Host "找到以下 Qt 安装:" -ForegroundColor Green
for ($i = 0; $i -lt $qtInstallations.Count; $i++) {
    $qt = $qtInstallations[$i]
    $version = Get-QtVersion $qt.QMake
    $hasMySQL = Test-MySQLDriver $qt.Path
    $status = if ($hasMySQL) { "✅ 已安装 MySQL 驱动" } else { "❌ 缺少 MySQL 驱动" }
    
    Write-Host "  [$($i+1)] $($qt.Version) (Qt $version) - $status" -ForegroundColor White
    Write-Host "      路径: $($qt.Path)" -ForegroundColor Gray
}

Write-Host ""

# 选择 Qt 版本
if ($qtInstallations.Count -eq 1) {
    $selectedQt = $qtInstallations[0]
    Write-Host "自动选择唯一的 Qt 安装: $($selectedQt.Version)" -ForegroundColor Cyan
} else {
    do {
        $choice = Read-Host "请选择要安装 MySQL 驱动的 Qt 版本 (1-$($qtInstallations.Count))"
        $choiceNum = [int]$choice - 1
    } while ($choiceNum -lt 0 -or $choiceNum -ge $qtInstallations.Count)
    
    $selectedQt = $qtInstallations[$choiceNum]
}

Write-Host ""
Write-Host "2. 检查选定的 Qt 安装..." -ForegroundColor Green
Write-Host "Qt 路径: $($selectedQt.Path)" -ForegroundColor White
$qtVersion = Get-QtVersion $selectedQt.QMake
Write-Host "Qt 版本: $qtVersion" -ForegroundColor White

# 检查是否已有 MySQL 驱动
if (Test-MySQLDriver $selectedQt.Path) {
    Write-Host "✅ MySQL 驱动已存在" -ForegroundColor Green
    Write-Host ""
    Write-Host "如果仍然无法使用 MySQL，可能需要:" -ForegroundColor Yellow
    Write-Host "1. 安装 MySQL 客户端库"
    Write-Host "2. 检查环境变量配置"
    Write-Host "3. 重启应用程序"
    Write-Host ""
    Write-Host "建议运行测试: test_connection.bat" -ForegroundColor Cyan
    exit 0
}

Write-Host ""
Write-Host "3. 准备安装 MySQL 驱动..." -ForegroundColor Green

# 创建临时目录
$tempDir = Join-Path $env:TEMP "qt_mysql_driver"
if (Test-Path $tempDir) {
    Remove-Item $tempDir -Recurse -Force
}
New-Item -ItemType Directory -Path $tempDir | Out-Null

Write-Host "临时目录: $tempDir" -ForegroundColor Gray

# 根据 Qt 版本选择驱动源
$driverSources = @{
    "6.5" = "https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.5/qsqlmysql.dll"
    "6.4" = "https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.4/qsqlmysql.dll"
    "6.3" = "https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt6.3/qsqlmysql.dll"
    "5.15" = "https://github.com/thecodemonkey86/qt_mysql_driver/releases/download/qt5.15/qsqlmysql.dll"
}

# 匹配版本
$majorMinor = $qtVersion -replace "^(\d+\.\d+).*", '$1'
$downloadUrl = $null

foreach ($version in $driverSources.Keys) {
    if ($majorMinor -eq $version) {
        $downloadUrl = $driverSources[$version]
        break
    }
}

if (-not $downloadUrl) {
    Write-Host "❌ 未找到适合 Qt $qtVersion 的预编译驱动" -ForegroundColor Red
    Write-Host ""
    Write-Host "建议解决方案:" -ForegroundColor Yellow
    Write-Host "1. 重新安装 Qt 并选择 MySQL 支持"
    Write-Host "2. 使用 SQLite 替代: run_with_sqlite.bat"
    Write-Host "3. 手动编译 MySQL 驱动"
    exit 1
}

Write-Host "找到适合的驱动: $downloadUrl" -ForegroundColor Green

# 下载驱动
$driverFile = Join-Path $tempDir "qsqlmysql.dll"
if (-not (Download-File $downloadUrl $driverFile)) {
    Write-Host "❌ 驱动下载失败" -ForegroundColor Red
    exit 1
}

Write-Host "✅ 驱动下载完成" -ForegroundColor Green

# 安装驱动
Write-Host ""
Write-Host "4. 安装 MySQL 驱动..." -ForegroundColor Green

$sqlDriversDir = Join-Path $selectedQt.Path "plugins\sqldrivers"
if (-not (Test-Path $sqlDriversDir)) {
    Write-Host "创建目录: $sqlDriversDir" -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $sqlDriversDir -Force | Out-Null
}

$targetPath = Join-Path $sqlDriversDir "qsqlmysql.dll"

try {
    Copy-Item $driverFile $targetPath -Force
    Write-Host "✅ MySQL 驱动安装成功" -ForegroundColor Green
    Write-Host "安装位置: $targetPath" -ForegroundColor Gray
}
catch {
    Write-Host "❌ 驱动安装失败: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "可能需要管理员权限" -ForegroundColor Yellow
    exit 1
}

# 清理临时文件
Remove-Item $tempDir -Recurse -Force

Write-Host ""
Write-Host "5. 检查 MySQL 客户端库..." -ForegroundColor Green

# 检查 MySQL 是否安装
$mysqlFound = $false
try {
    $null = Get-Command mysql.exe -ErrorAction Stop
    $mysqlFound = $true
    Write-Host "✅ 找到 MySQL 客户端" -ForegroundColor Green
}
catch {
    Write-Host "❌ 未找到 MySQL 客户端" -ForegroundColor Red
}

if (-not $mysqlFound) {
    Write-Host ""
    Write-Host "MySQL 驱动需要 MySQL 客户端库支持" -ForegroundColor Yellow
    Write-Host "请安装以下之一:" -ForegroundColor Yellow
    Write-Host "1. MySQL Server: https://dev.mysql.com/downloads/mysql/"
    Write-Host "2. MySQL Connector/C: https://dev.mysql.com/downloads/connector/c/"
    Write-Host ""
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "安装完成！" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "下一步:" -ForegroundColor Yellow
Write-Host "1. 重启你的 Qt 应用程序"
Write-Host "2. 运行测试: test_connection.bat"
Write-Host "3. 如果仍有问题，检查 MySQL 客户端库安装"
Write-Host ""
Write-Host "如果遇到问题，可以使用 SQLite 替代: run_with_sqlite.bat" -ForegroundColor Cyan