QT += testlib sql widgets charts
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# Include paths
INCLUDEPATH += ../include

# Source files
SOURCES += test_cascade_delete.cpp \
    ../src/databasemanager.cpp

# Header files
HEADERS += ../include/databasemanager.h

# Target
TARGET = test_cascade_delete
