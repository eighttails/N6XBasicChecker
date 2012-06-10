#-------------------------------------------------
#
# Project created by QtCreator 2012-05-13T16:54:21
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_libn6xbasiccheckertesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#GCC4.7でコンパイルが通るようになるまでの暫定措置
QMAKE_CXX = /usr/bin/g++-4.6

SOURCES += \
    tst_libn6xbasiccheckertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/release/ -llibN6XBasicChecker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/debug/ -llibN6XBasicChecker
else:symbian: LIBS += -llibN6XBasicChecker
else:unix: LIBS += -L$$OUT_PWD/../libN6XBasicChecker/ -llibN6XBasicChecker

INCLUDEPATH += $$PWD/../libN6XBasicChecker
DEPENDPATH += $$PWD/../libN6XBasicChecker

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/release/libN6XBasicChecker.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/debug/libN6XBasicChecker.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/liblibN6XBasicChecker.a
