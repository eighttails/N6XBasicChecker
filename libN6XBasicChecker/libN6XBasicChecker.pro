#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = libN6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib

#4.7でコンパイルが通るようになるまでの暫定措置
QMAKE_CXX = /usr/bin/g++-4.6

PRECOMPILED_HEADER = pch.h
QMAKE_CXXFLAGS = -std=c++0x

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    checker.h

SOURCES += \
    checker.cpp


