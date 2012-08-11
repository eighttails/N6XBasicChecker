#-------------------------------------------------
#
# Project created by QtCreator 2012-05-13T16:54:21
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_libn6xbasiccheckertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_libn6xbasiccheckertest.cpp

#WindowsではBOOSTDIR環境変数をセット
win32{
INCLUDEPATH += $(BOOSTDIR)/include/boost-1_50
QMAKE_LFLAGS = -static-libgcc -static-libstdc++
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/release/ -lN6XBasicChecker -L$(BOOSTDIR)/lib/ -lboost_regex-mgw47-mt-1_50
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/debug/ -lN6XBasicChecker -L$(BOOSTDIR)/lib/ -lboost_regex-mgw47-mt-d-1_50
else:symbian: LIBS += -llibN6XBasicChecker
else:unix: LIBS += -L$$OUT_PWD/../libN6XBasicChecker/ -lN6XBasicChecker -lboost_regex

INCLUDEPATH += $$PWD/../libN6XBasicChecker $$PWD/../babel
DEPENDPATH += $$PWD/../libN6XBasicChecker

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/release/libN6XBasicChecker.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/debug/libN6XBasicChecker.a
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/libN6XBasicChecker.a
