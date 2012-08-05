#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = N6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib

#WindowsではBOOSTDIR環境変数をセット
win32:INCLUDEPATH += $(BOOSTDIR)/include/boost-1_50

INCLUDEPATH += $$PWD/../babel

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    checker.h \
    babelwrap.h \
    spiritwrap.h \
    checkercommon.h \
    parserstatus.h

SOURCES += \
    checker.cpp \
    checker2.cpp \
    babelwrap.cpp \
    parserstatus.cpp


