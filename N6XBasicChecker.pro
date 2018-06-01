TEMPLATE = subdirs

SUBDIRS += \
    libN6XBasicChecker \
    libN6XBasicCheckerTest \
    libtxt2bas \
    N6XBasicCheckerMain \
    txt2bas \
    bas2txt

N6XBasicCheckerMain.depends = libN6XBasicChecker libtxt2bas
libN6XBasicCheckerTest.depends = libN6XBasicChecker
txt2bas.depends = libtxt2bas
