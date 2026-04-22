QT += core gui widgets sql

CONFIG += c++17 console

TARGET = verify_widget

SOURCES += \
    verify_widget_implementation.cpp \
    ../src/medicalrecordwidget.cpp \
    ../src/medicalrecordmodel.cpp \
    ../src/databasemanager.cpp

HEADERS += \
    ../include/medicalrecordwidget.h \
    ../include/medicalrecordmodel.h \
    ../include/databasemanager.h

FORMS += \
    ../ui/medicalrecordwidget.ui

INCLUDEPATH += ../include