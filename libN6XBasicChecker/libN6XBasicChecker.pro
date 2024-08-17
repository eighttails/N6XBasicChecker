#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T21:58:16
#
#-------------------------------------------------
TARGET = N6XBasicChecker
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

win32{
#WindowsではBOOSTDIR環境変数をセット
INCLUDEPATH += $(BOOSTDIR)/include
#コンパイル時のメモリ対策
QMAKE_CXXFLAGS += --param ggc-min-expand=25 --param ggc-min-heapsize=8192 -Wa,-mbig-obj
}
INCLUDEPATH += $$PWD/../babel

HEADERS += \
    babelwrap.h \
    checker.h \
    checkercommon.h \
    parserstatus.h \
    spiritwrap.h

SOURCES += \
    babelwrap.cpp \
    checker.cpp \
    parserstatus.cpp


