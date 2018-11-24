#!/bin/bash

#依存ライブラリ
pacman -S --needed --noconfirm \
    $MINGW_PACKAGE_PREFIX-toolchain \
    $MINGW_PACKAGE_PREFIX-qt5 \
    $MINGW_PACKAGE_PREFIX-boost 

export SCRIPT_DIR=$(dirname $(readlink -f ${BASH_SOURCE:-$0}))

#並列ビルド
MINGW32MAKE="mingw32-make -j$NUMBER_OF_PROCESSORS"

N6XBC_DBUILD_DIR=N6XBasicChecker-build-$MINGW_CHOST

if [ -e $N6XBC_DBUILD_DIR ]; then 
	rm -rf $N6XBC_DBUILD_DIR
fi
mkdir $N6XBC_DBUILD_DIR

cd $N6XBC_DBUILD_DIR
qmake $SCRIPT_DIR/../N6XBasicChecker.pro 
$MINGW32MAKE release

