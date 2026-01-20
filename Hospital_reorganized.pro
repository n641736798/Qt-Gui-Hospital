QT += core gui sql
# Enable Qt Charts module for ECG dashboard
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

## NO_QT_CHARTS was previously used to disable charts when the module
## was unavailable. It is now removed to enable real ECG charts.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 源文件路径
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/databasemanager.cpp \
    src/patientdialog.cpp \
    src/ecgdatathread.cpp \
    src/medicalrecordmodel.cpp \
    src/medicalrecordwidget.cpp \
    src/medicalrecorddialog.cpp

# 头文件路径
HEADERS += \
    include/mainwindow.h \
    include/databasemanager.h \
    include/patientdialog.h \
    include/ecgdatathread.h \
    include/medicalrecordmodel.h \
    include/medicalrecordwidget.h \
    include/medicalrecorddialog.h

# UI文件路径
FORMS += \
    ui/mainwindow.ui \
    ui/medicalrecordwidget.ui \
    ui/medicalrecorddialog.ui

# 资源文件路径
RESOURCES += \
    resources/resources.qrc

# 包含路径
INCLUDEPATH += include

# Default rules for deployment.
qnx: target.path = /tmp/${TARGET}/bin
else: unix:!android: target.path = /opt/${TARGET}/bin
!isEmpty(target.path): INSTALLS += target