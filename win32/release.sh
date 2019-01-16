#!/bin/bash

#環境チェック
if [ -z "$MINGW_PREFIX" ]; then
	echo "Please run this script in MinGW 32bit or 64bit shell. (not in MSYS2 shell)"
	exit 1
fi

if [ "$MINGW_CHOST" = "i686-w64-mingw32" ]; then
	BIT=32bit
else
	BIT=64bit
fi

export SCRIPT_DIR=$(dirname $(readlink -f ${BASH_SOURCE:-$0}))

#引数としてバージョンを指定すること
if [ -z $1 ]; then
    VERSION=dev
else
    VERSION=$1
fi

RELEASE_DIR=$SCRIPT_DIR/../../N6XBasicChecker_release
#Windows版バイナリ配置先
WIN_BIN_NAME=N6XBasicChecker_$VERSION\_win_$BIT
WIN_BIN_DIR=$RELEASE_DIR/$WIN_BIN_NAME
rm -rf $WIN_BIN_DIR
mkdir -p $WIN_BIN_DIR
pushd $RELEASE_DIR

#プログラム本体をビルド
$SCRIPT_DIR/buildrelease.sh "$PWD"

pushd N6XBasicChecker-build-$MINGW_CHOST
cp -f N6XBasicCheckerMain/release/N6XBasicChecker.exe $WIN_BIN_DIR
cp -f txt2bas/release/txt2bas.exe $WIN_BIN_DIR
cp -f bas2txt/release/bas2txt.exe $WIN_BIN_DIR
cp -f $SCRIPT_DIR/../*.txt $WIN_BIN_DIR
popd

zip -r $WIN_BIN_NAME.zip $WIN_BIN_NAME
popd

#ソースtarball
pushd $SCRIPT_DIR/..
SOURCENAME=N6XBasicChecker_$VERSION\_src
git archive --format=tar --prefix=$SOURCENAME/ HEAD | gzip > $RELEASE_DIR/$SOURCENAME.tar.gz
popd
