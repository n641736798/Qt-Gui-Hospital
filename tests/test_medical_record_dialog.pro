QT += core gui widgets sql testlib

CONFIG += c++17 testcase

TARGET = test_medical_record_dialog

SOURCES += \
    test_medical_record_dialog.cpp \
    ../src/medicalrecorddialog.cpp \
    ../src/databasemanager.cpp

HEADERS += \
    ../include/medicalrecorddialog.h \
    ../include/databasemanager.h

FORMS += \
    ../ui/medicalrecorddialog.ui

INCLUDEPATH += ../include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target