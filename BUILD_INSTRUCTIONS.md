# 构建说明 / Build Instructions

## QCustomPlot 依赖（首次构建前必做）

本项目使用 **项目内的** QCustomPlot 源码（`include/qcustomplot.h`、`src/qcustomplot.cpp`），不引用外部路径。

**首次构建前**，请将 QCustomPlot 拷贝到项目目录：

1. 在项目根目录双击运行 **`copy_qcustomplot.bat`**  
   - 脚本会从 `D:\software\Qt\additional part\qcustomplot-source\` 复制 `qcustomplot.h` → `include/`，`qcustomplot.cpp` → `src/`  
   - 若你的 QCustomPlot 在其他路径，可编辑脚本中的 `SRC` 变量后重新运行  

2. 拷贝完成后，用 Qt Creator 或 qmake 正常构建即可。

## 问题说明
您的系统配置了 conda 的 Qt (MSVC 版本)，但尝试使用 MinGW 编译器构建项目，导致库链接错误。

## 解决方案

### 方案 1: 使用 Qt Creator (推荐)

1. 打开 Qt Creator
2. 点击 "文件" -> "打开文件或项目"
3. 选择 `Hospital_reorganized.pro` 文件
4. 在 "Configure Project" 页面，选择合适的 Kit (工具链)
5. 点击左下角的 "构建" 按钮 (锤子图标) 或按 Ctrl+B

Qt Creator 会自动配置正确的编译器和库路径。

### 方案 2: 安装 Visual Studio

1. 下载并安装 Visual Studio Community (免费)
2. 确保安装了 "使用 C++ 的桌面开发" 工作负载
3. 打开 "x64 Native Tools Command Prompt for VS"
4. 导航到项目目录
5. 运行:
   ```
   qmake Hospital_reorganized.pro
   nmake
   ```

### 方案 3: 安装 MinGW 版本的 Qt

1. 从 Qt 官网下载 MinGW 版本的 Qt
2. 安装后，使用该版本的 qmake
3. 运行:
   ```
   qmake Hospital_reorganized.pro
   mingw32-make
   ```

## 代码修复完成

所有代码错误已修复:
- ✅ 类型转换错误
- ✅ 废弃的 API 调用
- ✅ 未使用的变量警告
- ✅ 初始化列表兼容性
- ✅ 空指针检查

剩余的问题仅是编译器与 Qt 库版本不匹配的构建系统问题。

## 推荐操作

**最简单的方法**: 使用 Qt Creator 打开项目并构建。
