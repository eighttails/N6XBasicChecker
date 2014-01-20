#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T22:59:40
#
#-------------------------------------------------


QT       -= core gui

TARGET = N6XBasicChecker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

#WindowsではBOOSTDIR環境変数をセット
win32{
INCLUDEPATH += $(BOOSTDIR)/include
QMAKE_LFLAGS = -static-libgcc -static-libstdc++
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/release/ -lN6XBasicChecker -L$(BOOSTDIR)/lib/ -lboost_program_options-mt -lboost_regex-mt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libN6XBasicChecker/debug/ -lN6XBasicChecker -L$(BOOSTDIR)/lib/ -lboost_program_optionsmt-d -lboost_regex-mt-d
else:symbian: LIBS += -lN6XBasicChecker
else:unix: LIBS += -L$$OUT_PWD/../libN6XBasicChecker/ -lN6XBasicChecker -lboost_program_options -lboost_regex

INCLUDEPATH += $$PWD/../libN6XBasicChecker $$PWD/../babel
DEPENDPATH += $$PWD/../libN6XBasicChecker

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/release/libN6XBasicChecker.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/debug/libN6XBasicChecker.a
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../libN6XBasicChecker/libN6XBasicChecker.a
