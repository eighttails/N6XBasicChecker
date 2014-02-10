#-------------------------------------------------
#
# Project created by QtCreator 2014-01-25T22:05:39
#
#-------------------------------------------------

QT       -= core gui

TARGET = txt2bas
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    buffer.c \
    parse5.c \
    parse6.c \
    txt2bas.c

HEADERS += \
    txt2bas.h \
    sjis.h \
    p6code6.h \
    p6code5.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
