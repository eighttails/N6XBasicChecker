TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

win32{
QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++
}

SOURCES += \
    p6top6t.c

