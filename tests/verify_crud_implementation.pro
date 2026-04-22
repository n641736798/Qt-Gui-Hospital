QT += core widgets sql testlib
CONFIG += testcase

TARGET = verify_crud_implementation
TEMPLATE = app

# Include paths
INCLUDEPATH += ../include

# Source files
SOURCES += verify_crud_implementation.cpp \
           ../src/medicalrecordwidget.cpp \
           ../src/medicalrecordmodel.cpp \
           ../src/medicalrecorddialog.cpp \
           ../src/databasemanager.cpp

# Header files  
HEADERS += ../include/medicalrecordwidget.h \
           ../include/medicalrecordmodel.h \
           ../include/medicalrecorddialog.h \
           ../include/databasemanager.h

# UI files
FORMS += ../ui/medicalrecordwidget.ui \
         ../ui/medicalrecorddialog.ui

# Output directory
DESTDIR = ./