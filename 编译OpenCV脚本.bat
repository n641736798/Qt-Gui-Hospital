@echo off
chcp 65001
cls
echo ==========================================
echo  OpenCV 4.12.0 编译脚本 (MinGW 15.2.0)
echo ==========================================
echo.

:: 设置编译环境
set MINGW_PATH=D:\software\mingw64
set CMAKE_PATH=D:\software\CMAKE\bin
set OPENCV_SRC=D:\software\opencv-4.12.0
set OPENCV_BUILD=D:\software\opencv-4.12.0-build
set OPENCV_INSTALL=D:\software\opencv-mingw1520

:: 设置 PATH
set PATH=%MINGW_PATH%\bin;%CMAKE_PATH%;%PATH%

echo 编译配置:
echo   MinGW: %MINGW_PATH%
echo   CMake: %CMAKE_PATH%
echo   源码: %OPENCV_SRC%
echo   构建: %OPENCV_BUILD%
echo   安装: %OPENCV_INSTALL%
echo.

:: 检查 GCC 版本
echo 检查 GCC 版本...
gcc --version
echo.

:: 创建构建目录
if not exist "%OPENCV_BUILD%" mkdir "%OPENCV_BUILD%"
if not exist "%OPENCV_INSTALL%" mkdir "%OPENCV_INSTALL%"

cd /d "%OPENCV_BUILD%"

echo 正在配置 CMake...
cmake -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=%OPENCV_INSTALL% ^
  -DWITH_CUDA=OFF ^
  -DWITH_OPENCL=OFF ^
  -DBUILD_SHARED_LIBS=ON ^
  -DBUILD_opencv_world=ON ^
  -DBUILD_TESTS=OFF ^
  -DBUILD_PERF_TESTS=OFF ^
  -DBUILD_EXAMPLES=OFF ^
  -DBUILD_DOCS=OFF ^
  -DWITH_FFMPEG=OFF ^
  -DWITH_GSTREAMER=OFF ^
  -DWITH_VTK=OFF ^
  -DWITH_QT=OFF ^
  -DWITH_GTK=OFF ^
  -DWITH_OPENGL=OFF ^
  -DWITH_EIGEN=OFF ^
  -DWITH_TBB=OFF ^
  -DWITH_IPP=OFF ^
  -DWITH_LAPACK=OFF ^
  -DWITH_PROTOBUF=OFF ^
  -DWITH_V4L=OFF ^
  -DWITH_DSHOW=ON ^
  -DWITH_MSMF=OFF ^
  -DBUILD_opencv_python2=OFF ^
  -DBUILD_opencv_python3=OFF ^
  -DBUILD_JAVA=OFF ^
  %OPENCV_SRC%

if errorlevel 1 (
    echo CMake 配置失败！
    pause
    exit /b 1
)

echo.
echo CMake 配置完成！
echo.
echo 开始编译...
echo 这可能需要 30-60 分钟，请耐心等待...
echo.

mingw32-make -j4

if errorlevel 1 (
    echo 编译失败！
    pause
    exit /b 1
)

echo.
echo 编译完成！
echo.
echo 开始安装...

mingw32-make install

if errorlevel 1 (
    echo 安装失败！
    pause
    exit /b 1
)

echo.
echo ==========================================
echo  OpenCV 4.12.0 编译安装完成！
echo ==========================================
echo.
echo 安装路径: %OPENCV_INSTALL%
echo.
echo 请修改 Hospital_reorganized.pro 文件:
echo   OPENCV_PATH = %OPENCV_INSTALL%
echo.
pause
