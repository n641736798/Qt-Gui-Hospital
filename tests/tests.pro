QT += core sql testlib widgets
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# Test configuration
TARGET = database_connection_tests
CONFIG += c++17

# Include paths
INCLUDEPATH += ../include

# Source files
SOURCES += \
    test_database_connection.cpp \
    ../src/databasemanager.cpp

# Header files  
HEADERS += \
    ../include/databasemanager.h

# Test data and resources
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# Output directory
DESTDIR = ../build/tests
OBJECTS_DIR = ../build/tests/obj
MOC_DIR = ../build/tests/moc