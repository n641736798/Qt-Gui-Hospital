QT += core gui widgets sql testlib

CONFIG += c++17 testcase

TARGET = test_medical_record_widget

SOURCES += \
    test_medical_record_widget.cpp \
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

# Generate UI headers
UI_DIR = .