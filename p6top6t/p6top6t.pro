TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

win32{
QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++
}

SOURCES += \
    p6top6t.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/release/ -lN6XBasicChecker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/debug/ -lN6XBasicChecker
else:unix: LIBS += -L$$OUT_PWD/../libN6XBasicChecker/ -lN6XBasicChecker

INCLUDEPATH += $$PWD/../libN6XBasicChecker $$PWD/../babel
DEPENDPATH += $$PWD/../libN6XBasicChecker

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/release/libN6XBasicChecker.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/debug/libN6XBasicChecker.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/release/N6XBasicChecker.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/debug/N6XBasicChecker.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/libN6XBasicChecker.a
