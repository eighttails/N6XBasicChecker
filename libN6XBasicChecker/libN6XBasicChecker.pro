#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = libN6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib

PRECOMPILED_HEADER = pch.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    pch.h \
    checker.h

SOURCES += \
    checker.cpp


