QT += core gui sql printsupport charts
# 同时支持QCustomPlot和QtChart两种渲染引擎，可通过配置文件切换
# printsupport: QCustomPlot uses QPrinter (PDF/print export)

# OpenCV 4.12 支持（医学影像查看功能）
# 使用重新编译的 MinGW 15.2.0 版本
win32 {
    OPENCV_PATH = D:/software/opencv-mingw1520
    INCLUDEPATH += $$OPENCV_PATH/include
    
    # MinGW 版本的 OpenCV 库路径
    LIBS += -L$$OPENCV_PATH/x64/mingw/lib -lopencv_world4120
}
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# MinGW: 解决 qcustomplot.cpp 编译时 "file too big" / "too many sections" 错误
win32-g++: QMAKE_CXXFLAGS += -Wa,-mbig-obj

## NO_QT_CHARTS was previously used to disable charts when the module
## was unavailable. It is now removed to enable real ECG charts.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 源文件路径
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/core/databasemanager.cpp \
    src/core/patientdialog.cpp \
    src/core/patientmodel.cpp \
    src/core/patientwidget.cpp \
    src/core/medicalrecordmodel.cpp \
    src/core/medicalrecordwidget.cpp \
    src/core/medicalrecorddialog.cpp \
    src/chart/mainwindow_chart.cpp \
    src/chart/ecgdatathread.cpp \
    src/chart/qcustomplotchart_adapter.cpp \
    src/chart/qtchart_adapter.cpp \
    src/image/medicalimageviewer.cpp \
    src/image/imageviewerdialog.cpp \
    src/image/documentscannerdialog.cpp \
    src/image/morphologydialog.cpp \
    src/third_party/qcustomplot.cpp

# 头文件路径（qcustomplot.h 须在 HEADERS 中，以便对 Q_OBJECT/signals 运行 MOC）
HEADERS += \
    include/mainwindow.h \
    include/core/databasemanager.h \
    include/core/patientdialog.h \
    include/core/patientmodel.h \
    include/core/patientwidget.h \
    include/core/medicalrecordmodel.h \
    include/core/medicalrecordwidget.h \
    include/core/medicalrecorddialog.h \
    include/chart/ecgdatathread.h \
    include/chart/iecgchart.h \
    include/chart/qcustomplotchart_adapter.h \
    include/chart/qtchart_adapter.h \
    include/image/medicalimageviewer.h \
    include/image/imageviewerdialog.h \
    include/image/documentscannerdialog.h \
    include/image/morphologydialog.h \
    include/third_party/qcustomplot.h

# UI文件路径
FORMS += \
    ui/mainwindow.ui \
    ui/medicalrecordwidget.ui \
    ui/medicalrecorddialog.ui

# 资源文件路径
RESOURCES += \
    resources/resources.qrc

# 包含路径
INCLUDEPATH += include \
    include/core \
    include/chart \
    include/image \
    include/third_party

# Default rules for deployment.
qnx: target.path = /tmp/${TARGET}/bin
else: unix:!android: target.path = /opt/${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
