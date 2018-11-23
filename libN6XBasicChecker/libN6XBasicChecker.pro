#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = N6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib

#コンパイル時のメモリ削減
QMAKE_CXXFLAGS += -Wa,-mbig-obj

#WindowsではBOOSTDIR環境変数をセット
win32{
INCLUDEPATH += $(BOOSTDIR)/include
}
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
    babelwrap.cpp \
    parserstatus.cpp


