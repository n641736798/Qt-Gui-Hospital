QT += core gui sql printsupport
# ECG dashboard uses QCustomPlot; Qt Charts removed.
# printsupport: QCustomPlot uses QPrinter (PDF/print export)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# MinGW: 解决 qcustomplot.cpp 编译时 "file too big" / "too many sections" 错误
win32-g++: QMAKE_CXXFLAGS += -Wa,-mbig-obj

## NO_QT_CHARTS was previously used to disable charts when the module
## was unavailable. It is now removed to enable real ECG charts.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 源文件路径（qcustomplot 使用项目内拷贝，见 copy_qcustomplot.bat）
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/databasemanager.cpp \
    src/patientdialog.cpp \
    src/ecgdatathread.cpp \
    src/medicalrecordmodel.cpp \
    src/medicalrecordwidget.cpp \
    src/medicalrecorddialog.cpp \
    src/qcustomplot.cpp

# 头文件路径（qcustomplot.h 须在 HEADERS 中，以便对 Q_OBJECT/signals 运行 MOC）
HEADERS += \
    include/mainwindow.h \
    include/databasemanager.h \
    include/patientdialog.h \
    include/ecgdatathread.h \
    include/medicalrecordmodel.h \
    include/medicalrecordwidget.h \
    include/medicalrecorddialog.h \
    include/qcustomplot.h

# UI文件路径
FORMS += \
    ui/mainwindow.ui \
    ui/medicalrecordwidget.ui \
    ui/medicalrecorddialog.ui

# 资源文件路径
RESOURCES += \
    resources/resources.qrc

# 包含路径（qcustomplot.h 在 include/ 下）
INCLUDEPATH += include

# Default rules for deployment.
qnx: target.path = /tmp/${TARGET}/bin
else: unix:!android: target.path = /opt/${TARGET}/bin
!isEmpty(target.path): INSTALLS += target