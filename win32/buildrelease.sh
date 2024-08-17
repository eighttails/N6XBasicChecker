#!/bin/bash
export SCRIPT_DIR=$(dirname $(readlink -f ${BASH_SOURCE:-$0}))

#依存ライブラリ
pacman -S --needed --noconfirm \
    --disable-download-timeout \
    $MINGW_PACKAGE_PREFIX-toolchain \
    $MINGW_PACKAGE_PREFIX-boost \
    2>/dev/null

#Qt6をインストール
$SCRIPT_DIR/MSYS2Private/qt6-static-private/qt.sh

#並列ビルド
MINGW32MAKE="mingw32-make -j$NUMBER_OF_PROCESSORS"

if [ -z $1 ]; then
    cd $SCRIPT_DIR/../../
else
    cd $1 
fi

N6XBC_DBUILD_DIR=N6XBasicChecker-build-$MINGW_CHOST

if [ -e $N6XBC_DBUILD_DIR ]; then 
	rm -rf $N6XBC_DBUILD_DIR
fi
mkdir $N6XBC_DBUILD_DIR

cd $N6XBC_DBUILD_DIR
$MINGW_PREFIX/local/qt6-static-private/bin/qmake $SCRIPT_DIR/../N6XBasicChecker.pro 
$MINGW32MAKE release

