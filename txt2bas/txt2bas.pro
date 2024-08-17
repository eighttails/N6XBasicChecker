TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle qt

INCLUDEPATH += $$PWD/../libtxt2bas

SOURCES += \
    txt2bas.c

win32{
QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libtxt2bas/release/ -ltxt2bas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libtxt2bas/debug/ -ltxt2bas
else:unix: LIBS += -L$$OUT_PWD/../libtxt2bas/ -ltxt2bas

INCLUDEPATH += $$PWD/../libtxt2bas
DEPENDPATH += $$PWD/../libtxt2bas

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtxt2bas/release/libtxt2bas.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtxt2bas/debug/libtxt2bas.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtxt2bas/release/txt2bas.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtxt2bas/debug/txt2bas.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libtxt2bas/libtxt2bas.a
