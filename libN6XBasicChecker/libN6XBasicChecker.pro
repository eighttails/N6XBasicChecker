#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------

QT       -= core gui

TARGET = N6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib


win32{
#WindowsではBOOSTDIR環境変数をセット
INCLUDEPATH += $(BOOSTDIR)/include
#コンパイル時のメモリ対策
QMAKE_CXXFLAGS += --param ggc-min-expand=25 --param ggc-min-heapsize=8192 -Wa,-mbig-obj
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


