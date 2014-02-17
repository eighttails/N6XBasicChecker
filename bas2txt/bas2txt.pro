TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    bas2txt.c \
    buffer.c \
    parse5.c \
    parse6.c

HEADERS += \
    sjis.h \
    bas2txt.h \
    p6code5.h \
    p6code6.h

