// ***************************************************
//  P6toP6T Ver.2.3                                 *
//  2013.12.31  by Yumitaro                         *
//  2021.06.08  by eighttails                       *
// ***************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "babelwrap.h"

#define VERSION 2.3

// P6T形式フォーマットVer.2
// 基本的には「ベタイメージ+フッタ+ベタイメージサイズ(4byte)」という構造
// フッタにはDATAブロック情報が含まれる。
// DATAブロックは無音部を区切りとして1と数える。
// BASICなど「ヘッダ＋データ」という構造の場合，DATAブロックは2個となるが
// ID番号を同一にすることで関連付けて管理できる。
// ボーレートはとりあえず1200ボー固定
//
// [フッタ]
// header (2byte) : "P6"
// ver    (1byte) : バージョン
// dbnum  (1byte) : 含まれるDATAブロック数(255個まで)
// start  (1byte) : オートスタートフラグ(0:無効 1:有効)
// basic  (1byte) : BASICモード(PC-6001の場合は無意味)
// page   (1byte) : ページ数
// askey  (2byte) : オートスタートコマンド文字数
// ...コマンドがある場合はこの後にaskey分続く
// exhead (2byte) : 拡張情報サイズ(64KBまで)
// ...拡張情報がある場合はこの後にexhead分続く
//
// [DATAブロック]
// header (2byte) : "TI"
// id     (1byte) : ID番号(DATAブロックを関連付ける)
// name  (16byte) : データ名(15文字+'00H')
// baud   (2byte) : ボーレート(600/1200)
// stime  (2byte) : 無音部の時間(ms)
// ptime  (2byte) : ぴー音の時間(ms)
// start  (4byte) : ベタイメージ先頭からのオフセット
// size   (4byte) : データサイズ



// DATAブロック情報構造体
typedef struct{
	char header[3];			// 識別子 "TI"
	uint8_t id;				// ID番号
	char name[16];			// データ名(15文字+'00H')
	uint16_t baud;			// ボーレート(600/1200)
	uint16_t stime;			// 無音部の時間(ms)
	uint16_t ptime;			// ぴー音の時間(ms)
	uint32_t start;			// ベタイメージ先頭からのオフセット
	uint32_t size;			// データサイズ

	uint8_t *data;			// データ領域へのポインタ(オンメモリで扱う場合)

} P6DATA;

// P6T情報構造体
typedef struct{
	char header[3];			// 識別子 "P6"
	uint8_t ver;			// バージョン
	uint8_t dbnum;			// 含まれるDATAブロック数(255個まで)
	uint8_t start;			// オートスタートフラグ(0:無効 1:有効)
	uint8_t basic;			// BASICモード(PC-6001の場合は無意味)
	uint8_t page;			// ページ数
	uint16_t askey;			// オートスタートコマンドサイズ
	uint16_t exhead;		// 拡張情報サイズ(64KBまで)

	char fname[260];		// P6T ファイル名
	FILE *fp;				// TAPE ファイルポインタ
	uint8_t *exh;			// 拡張情報格納領域へのポインタ
	char *ask;				// オートスタートコマンド格納領域へのポインタ
	P6DATA *db;				// DATAブロック情報へのポインタ
	uint32_t beta;			// ベタイメージサイズ
	int mode;				// OPENモード(TAPE_LOAD/TAPE_SAVE)
	int dbsel;				// 選択しているDATAブロック番号(0-254)
	int seek;				// 選択しているDATAブロックのシーク位置(データ領域先頭が0)
	int swait;				// 無音部の待ち回数
	int pwait;				// ぴー音の待ち回数
} P6CAS;

// テープの種類
const char *TapeType[] = {
	"= QUIT =",
	"ベタ",
	"BASIC",
	"I/Oモニタ",
	"インテルHEX(MON)",
};

#define	NumOfType	5
#define	StdP		3400
#define	IOP			3400
#define	BAS			50
#define	IOS			1000

#define	StdBaud		1200


P6CAS *fout=NULL;		// P6T情報ポインタ
P6DATA *fdb[255];		// DATAブロック情報ポインタ配列

FILE *infp = NULL;		// 入力ファイルポインタ
uint32_t fsize = 0;		// 入力ファイルのサイズ
uint8_t idnum = 0;		// DATAブロックのID番号

FILE *inparam = stdin;	// パラメータ入力元(デフォルトでは標準入力)
FILE *outparam = NULL;	// パラメータ出力先
const char *paramfile = "p6tparams.txt";	// パラメータファイル名

// gets()代替関数
char* gets_f(char* s, int n, FILE *fp)
{
	// バッファサイズが1に満たない場合はNULLを返す
	if (n < 1) return NULL;
	memset(s, 0, n);
	// 1文字読み込み。EOFの場合はNULLを返す
	int i = 0;
	for(; i < n; i++) {
		int ch = fgetc( fp );
		// 自動入力の場合は標準出力に表示
		if (inparam != stdin) putchar(ch);
		if (ch == EOF || ch == '\n' || ch == '\0' ) {
			break;
		}
		s[i] = ch;
	}
	// パラメータファイルに書き出し
	if ( outparam ){
		// 書き出す際は終端を改行にする
		s[i] = '\n';
		fwrite( s, i + 1, 1, outparam );
	}
	// 改行を終端記号に置換
	s[i] = '\0';
	return i > 0 ? s : NULL;
}

// getchar_f( inparam )代替関数
int getchar_f( FILE *fp )
{
	int ch = fgetc( fp );
	// 自動入力の場合は標準出力に表示
	if (inparam != stdin) putchar(ch);
	// パラメータファイルに書き出し
	if ( ch != EOF && outparam ){
		fputc( ch, outparam );
	}
	return ch;
}

// strcpy代替関数
// 1文字ずつコピーすることを保証するしてコピー元、コピー先が
// 重なる場合の未定義動作を防ぐ
char *strcpy_f(char *s1, const char *s2)
{
	char *p = s1;
	while( *s1++ = *s2++ );
	return p;
}

// DATAブロック情報 領域確保 & 初期化
P6DATA *get_dbblock( P6CAS *ft )
{
	if( !(fdb[ft->dbnum]=(P6DATA *)malloc( sizeof(P6DATA) )) ) exit( 1 );
	memset( fdb[ft->dbnum], 0, sizeof(P6DATA) );
	strncpy( fdb[ft->dbnum]->header, "TI", 2 );

	return fdb[ft->dbnum++];
}


// データサイズ入力
// 空Enterの場合はmaxを返す
uint32_t input_datasize( uint32_t max )
{
	uint32_t dsize=0;
	char tmp[80];	// サイズ入力用バッファ

	while( !dsize ){
		if( gets_f( tmp, sizeof(tmp), inparam ) )
			dsize = atol( tmp );
		else
			dsize = max;

		if( !dsize ) printf_local("入力が正しくありません。\n> ");
	}
	return dsize;
}


// ベタ 読込み
void read_beta( P6CAS *ft, FILE *infp, FILE *outfp )
{
	P6DATA *db;
	uint32_t bsize=0;
	int i;


	// サイズ入力
	printf_local( "サイズを入力して下さい。（全ての場合は何も入力せずにEnter）\n> " );
	bsize = input_datasize( fsize - ftell( infp ) );


	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// DATAブロック情報セット
	db->id     = idnum++;			// ID番号
	db->baud   = StdBaud;			// ボーレート
	db->stime  = StdP;				// 無音部の時間(ms)
	db->ptime  = StdP;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = bsize;				// データサイズ

	// 実データセット
	for( i=0; i<(int)bsize; i++ ) fputc( fgetc( infp ), outfp );
}


// BASIC 読込み
void read_basic( P6CAS *ft, FILE *infp, FILE *outfp )
{
	P6DATA *db;
	uint32_t bsize=0,fpnt;
	int i,sum=0;

	// *** まずはヘッダ部分 ***
	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// ヘッダD3H読み飛ばし
	while( fgetc( infp ) != 0xd3 );	// D3Hが見つかるまで空読み
	while( fgetc( infp ) == 0xd3 );	// D3H以外が見つかるまで空読み
	fseek( infp, -1, SEEK_CUR );	// 行き過ぎた分戻る

	// DATAブロック情報セット
	db->id     = idnum;				// ID番号
	fread( db->name, 1, 6, infp );	// データ名(BASICのファイル名)
	db->baud   = StdBaud;			// ボーレート
	db->stime  = StdP;				// 無音部の時間(ms)
	db->ptime  = StdP;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = 16;				// データサイズ

	// 実データセット
	for( i=0; i<10; i++ ) fputc( 0xd3, outfp );	// ヘッダD3H
	fwrite( db->name, 6, 1, outfp );			// BASICのファイル名


	// *** 次にデータ部分 ***
	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// ブロックサイズを調べてデータ領域確保
	fpnt = ftell( infp );			// 現在位置を保存
	do{								// 00Hを10個検出するまで空読み
		if( fgetc( infp ) == 0 ) sum++;
		else                     sum = 0;
		bsize++;
	}while( sum < 10 );
	fseek( infp, fpnt, SEEK_SET );	// 元の位置に戻る

	// DATAブロック情報セット
	db->id     = idnum++;			// ID番号
	db->baud   = StdBaud;			// ボーレート
	db->stime  = 0;					// 無音部の時間(ms)
	db->ptime  = BAS;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = bsize;				// データサイズ
	// 	db->size   = bsize+2;			// データサイズ(エンドマーク最後の2byteを含む)

	// 実データセット
	for( i=0; i<(int)bsize; i++ ) fputc( fgetc( infp ), outfp );

	// エンドマークの00Hが残っている場合の処理
	// 	if( fgetc( infp ) ) fseek( infp, -1, SEEK_CUR );	// 00Hでなければ戻る
	// 	if( fgetc( infp ) ) fseek( infp, -1, SEEK_CUR );	// 00Hでなければ戻る
	// 	fputc( 0, outfp );	// 00Hを出力
	// 	fputc( 0, outfp );	// 00Hを出力
}


// I/Oモニタ形式 読込み
void read_iomon( P6CAS *ft, FILE *infp, FILE *outfp )
{
	P6DATA *db;
	uint32_t bsize=0;
	int i;

	// *** まずはヘッダ部分 ***
	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// ヘッダ9CH読み飛ばし
	while( fgetc( infp ) != 0x9c );
	while( fgetc( infp ) != 0xaf );
	// ヘッダAFH読み飛ばし
	while( fgetc( infp ) == 0xaf );
	fseek( infp, 1, SEEK_CUR );

	bsize = ( getc(infp) | getc(infp)<<8 ) + 7;	// データ部分の実データサイズ取得(ヘッダ9CH*6+1(?)を加算)

	// DATAブロック情報セット
	db->id     = idnum;				// ID番号
	db->baud   = StdBaud;			// ボーレート
	fgets( db->name, 3, infp );		// データ名(2文字のファイル名)
	db->stime  = StdP;				// 無音部の時間(ms)
	db->ptime  = StdP;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = 17;				// データサイズ

	// 実データセット
	for( i=0; i<6; i++ ) fputc( 0x9c, outfp );			// ヘッダ9CH
	for( i=0; i<5; i++ ) fputc( 0xaf, outfp );			// ヘッダAFH
	fseek( infp, -6, SEEK_CUR );
	for( i=0; i<6; i++ ) fputc( fgetc( infp ), outfp );	// アドレス，サイズ，ファイル名


	// *** 次にデータ部分 ***
	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// DATAブロック情報セット
	db->id     = idnum++;			// ID番号
	db->baud   = StdBaud;			// ボーレート
	db->stime  = IOS;				// 無音部の時間(ms)
	db->ptime  = IOP;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = bsize;				// データサイズ

	// 実データセット
	for( i=0; i<(int)bsize; i++ ) fputc( fgetc( infp ), outfp );
}


// インテルHEX 読込み
void read_ihex( P6CAS *ft, FILE *infp, FILE *outfp )
{
	P6DATA *db;
	int i,sum=0;
	uint32_t bsize=0,fpnt;

	db = get_dbblock( ft );	// DATAブロック情報 領域確保 & 初期化

	// ヘッダ9CH検出
	while( fgetc( infp ) != 0x9c );
	fseek( infp, -1, SEEK_CUR );

	// ブロックサイズを調べる
	fpnt = ftell( infp );	// 現在位置を保存

	fseek( infp, 9, SEEK_CUR );
	// 30Hを16個検出するまで空読み
	do{
		sum = 0;
		for( i=0; i<16; i++ ){
			if( fgetc( infp ) == 0x30 ) sum++;
			else                        sum = 0;
		}
		if( sum==16 && (fgetc( infp )==0x1a) ) break;
		else fseek( infp, 29, SEEK_CUR );
	}while( 1 );
	bsize = ftell( infp ) - fpnt;
	fseek( infp, fpnt, SEEK_SET );


	// DATAブロック情報セット
	db->id     = idnum++;			// ID番号
	db->baud   = StdBaud;			// ボーレート
	db->stime  = StdP;				// 無音部の時間(ms)
	db->ptime  = StdP;				// ぴー音の時間(ms)
	db->start  = ftell( outfp );	// ベタイメージ先頭からのオフセット
	db->size   = bsize;				// データサイズ

	// 実データセット
	for( i=0; i<(int)bsize; i++ ) fputc( fgetc( infp ), outfp );
}





// P6T情報フッタ出力
void p6_footer( P6CAS *ft )
{
	// "P6"
	fputc( 'P', ft->fp );
	fputc( '6', ft->fp );
	// バージョン(2)
	fputc( 2, ft->fp );
	// 含まれるDATAブロック数
	fputc(  ft->dbnum, ft->fp );
	// オートスタートフラグ(0:無効 1:有効)
	fputc(  ft->start, ft->fp );
	// BASICモード(PC-6001の場合は無意味)
	fputc(  ft->basic, ft->fp );
	// ページ数
	fputc(  ft->page, ft->fp );
	// オートスタートコマンド文字数
	fputc(  ft->askey    &0xff, ft->fp );
	fputc( (ft->askey>>8)&0xff, ft->fp );
	// オートスタートコマンド
	if( ft->askey )
		ft->ask[ft->askey-1] = 0x0d;
	fwrite( ft->ask, ft->askey, 1, ft->fp );
	// 拡張情報サイズ
	fputc(  ft->exhead    &0xff, ft->fp );
	fputc( (ft->exhead>>8)&0xff, ft->fp );
	// 拡張情報
	if( ft->exhead )
		fwrite( ft->exh, ft->exhead, 1, ft->fp );
}


// DATAブロック情報出力
void db_info( P6DATA *db, FILE *fp )
{
	// "TI"
	fputc( 'T', fp );
	fputc( 'I', fp );
	// ID番号
	fputc( db->id, fp );
	// データ名
	fwrite( db->name, 16, 1, fp );
	// ボーレート
	fputc(  db->baud    &0xff, fp );
	fputc( (db->baud>>8)&0xff, fp );
	// 無音部の時間
	fputc(  db->stime    &0xff, fp );
	fputc( (db->stime>>8)&0xff, fp );
	// ぴー音の時間
	fputc(  db->ptime    &0xff, fp );
	fputc( (db->ptime>>8)&0xff, fp );
	// ベタイメージ先頭からのオフセット
	fputc(  db->start     &0xff, fp );
	fputc( (db->start>>8) &0xff, fp );
	fputc( (db->start>>16)&0xff, fp );
	fputc( (db->start>>24)&0xff, fp );
	// データサイズ
	fputc(  db->size     &0xff, fp );
	fputc( (db->size>>8) &0xff, fp );
	fputc( (db->size>>16)&0xff, fp );
	fputc( (db->size>>24)&0xff, fp );

	printf_local( "ID番号        :%d\n",         db->id );
	printf_local( "データ名      :%s\n",         db->name );
	printf_local( "無音部の時間  :%d(%04X)\n",   db->stime,  db->stime );
	printf_local( "ぴー音の時間  :%d(%04X)\n",   db->ptime,  db->ptime );
	printf_local( "オフセット    :%d(%08X)\n",   db->start,  db->start );
	printf_local( "データサイズ  :%d(%08X)\n",   db->size,   db->size );
}



int main( int argc, char **argv )
{
	int i;
	char *c;
	uint8_t type;			// テープの種類
	uint32_t fpnt=0;		// ファイルポインタ保存用
	uint32_t totalsize=0;	// ベタイメージのトータルサイズ

	printf_local( "=== P6toP6T2 ===\n" );

	po::options_description desc( "オプション", 200 );
	desc.add_options()
			( "help,h", "ヘルプを表示" )
			( "version,v", "バージョンを表示" )
			( "utf8,u", "出力をUTF-8でエンコード" )
			( "store,s", "実行時の設定を保存" )
			( "restore,r", "保存された設定で実行" )
			;
	po::options_description hidden( "不可視オプション" );
	hidden.add_options()
			( "input-file", po::value<std::vector<std::string> >(), "input file" )
			;

	// 無名のオプションはファイル名として処理される
	po::positional_options_description p;
	p.add( "input-file", -1 );

	po::options_description cmdline_options;
	cmdline_options.add( desc ).add( hidden );

	po::variables_map vm;
	// 構文エラー
	bool argError = false;
	try{
		po::store( po::command_line_parser( argc, argv ).
				   options( cmdline_options ).positional( p ).run(), vm );
		po::notify( vm );
	} catch (...){
		argError = true;
	}

	// 出力のエンコード設定
	if ( vm.count( "utf8" ) ) {
		utf8Output = true;
	}

	// バージョン情報
	if ( vm.count( "version" ) ) {
		std::cout << "p6top6t ver." << VERSION << std::endl
				  << "Copyright 2012-2021 Yumitaro(@Yumitaro60), Tadahito Yao(@eighttails)" << std::endl
				  << "http://eighttails.seesaa.net" << std::endl;
		return 0;
	}

	// ヘルプオプションが指定、またはファイル名が指定されていない場合はヘルプを表示
	if ( vm.count( "help" ) || !vm.count( "input-file" ) || argError ) {
		std::cout << utf8_to_local( "Usage: p6top6t ファイル名 [オプション]" ) << std::endl;
		std::stringstream s;
		s << desc;
		std::cout << utf8_to_local( s.str() );
		return 0;
	}

	if ( vm.count( "store" ) ){
		if ( vm.count( "restore" ) ){
			printf_local( "--store と --restore オプションは同時に使用できません。\n" );
			return -1;
		}
		outparam = fopen( paramfile, "w" );
		if ( !outparam )	{
			printf_local( "%s が作成できません。\n", paramfile );
			return -1;
		}
	}

	if ( vm.count( "restore" ) ){
		inparam = fopen( paramfile, "r" );
		if ( !inparam )	{
			printf_local( "%s が見つかりません。\n", paramfile );
			return -1;
		}
	}

	std::vector<std::string> fileNames = vm["input-file"].as<std::vector<std::string> >();
	if(fileNames.size() != 1){
		printf_local( "ファイル名は１つのみ指定可能です。");
		return -1;
	}

	std::string input_file = fileNames[0];
	printf_local( "'%s'の変換を開始します。\n", input_file.c_str() );

	// P6T情報 領域確保 & 初期化
	if( !(fout=(P6CAS *)malloc( sizeof(P6CAS) ))) exit( 1 );
	memset( fout, 0, sizeof(P6CAS) );
	strncpy( fout->header, "P6", 2 );
	// 出力ファイル名を設定
	strcpy_f( fout->fname, input_file.c_str() );
	strcpy_f( strrchr( fout->fname, '.' ), ".p6t" );

	// DATAブロック情報ポインタ配列 初期化
	for( i=0; i<255; i++ ) fdb[i] = NULL;


	// オートスタート選択
	do{
		printf_local( "オートスタートを有効にしますか？ (0:No 1:Yes)>" );
		fout->start = getchar_f( inparam ) - '0';
		getchar_f( inparam );	// Enter空読み
	}while( fout->start > 1 );

	if( fout->start ){
		// BASICモード選択
		do{
			printf_local( "BASICモードを選んでください (1-6)>" );
			fout->basic = getchar_f( inparam ) - '0';
			getchar_f( inparam );	// Enter空読み
		}while( fout->basic < 1 || fout->basic > 6 );

		// ページ数選択
		do{
			printf_local( "ページ数を選んでください (1-4)>" );
			fout->page = getchar_f( inparam ) - '0';
			getchar_f( inparam );	// Enter空読み
		}while( fout->page < 1 || fout->page > 4 );

		// オートスタートコマンド入力
		int asksize = sizeof(char)*512;
		if( !(fout->ask=(char*)malloc( asksize )) ) exit( 1 );	// とりあえず512byteあれば十分か?
		printf_local( "オートスタートコマンドを入力してください（改行は'\\n' LOAD終了待ちは'\\r' 末尾には不要）\n" );
		printf_local( "'CLOAD\\rRUN' の場合は何も入力せずにEnter\n> " );

		// 入力なければ"CLOAD RUN"
		if( !gets_f( fout->ask, asksize, inparam ) ) strcpy_f( fout->ask, "CLOAD\\rRUN" );
		// エスケープシーケンスの処理
		do{
			c = (char *)strchr( fout->ask, (int)'\\' );
			if( c ){
				switch( c[1] ){
				case 'n':	// 改行
					c[0] = 0x0d;
					strcpy_f( &c[1], &c[2] );
					break;
				case 'r':	// テープリレー待ち
					c[0] = 0x0a;
					strcpy_f( &c[1], &c[2] );
					break;
				default:
					printf_local( "!! '%c' 不正な制御文字です。", c[1] );
					exit( 1 );
				}
			}
		}while( c );

		fout->askey = strlen( fout->ask ) + 1;
	}

	// 入力ファイルを開く
	if( (infp=fopen(input_file.c_str(), "rb")) == NULL ){
		fprintf_local( stderr, "p6top6t2: 入力ファイルが開けません '%s'\n", argv[1] );
		exit( 1 );
	}
	// 入力ファイルサイズ取得
	fseek( infp, 0, SEEK_END );
	fsize = ftell( infp );
	fseek( infp, 0, SEEK_SET );

	// 出力ファイルを開く
	if( (fout->fp=fopen( fout->fname, "wb")) == NULL ){
		fprintf_local( stderr, "p6top6t2: 出力ファイルが開けません '%s'\n", fout->fname );
		exit( 1 );
	}


	do{
		printf_local("\n処理済 %d (/%d)\n\n", (int)fpnt, (int)fsize );
		// テープの種類を選択
		printf_local( "<< テープの種類を選んでください >>\n" );
		for( i=0; i<NumOfType; i++ )
			printf_local( "%d. %s\n", i, TapeType[i] );
		type = getchar_f( inparam ) - '0';
		getchar_f( inparam );	// Enter空読み

		// データブロック出力
		switch( type ){
		case 0:	// - QUIT -
			fsize = 0;
			break;
		case 1:	// BETA
			read_beta( fout, infp, fout->fp );
			break;
		case 2:	// BASIC
			read_basic( fout, infp, fout->fp );
			break;
		case 3:	// I/O
			read_iomon( fout, infp, fout->fp );
			break;
		case 4:	// インテルHEX
			read_ihex( fout, infp, fout->fp );
			break;
		}
		fpnt = ftell( infp );
	}while( fpnt > 0 && fpnt < fsize );

	fout->ver = 2;	// P6T Ver.2
	// P6T情報フッタ出力
	p6_footer( fout );

	// DATAブロック情報出力
	for( i=0; i<fout->dbnum; i++ ){
		printf_local( "<< Block No.%d >>\n", i );
		db_info( fdb[i], fout->fp );
		totalsize += fdb[i]->size;
	}

	// ベタイメージサイズ出力
	fputc(  totalsize     &0xff, fout->fp );
	fputc( (totalsize>>8) &0xff, fout->fp );
	fputc( (totalsize>>16)&0xff, fout->fp );
	fputc( (totalsize>>24)&0xff, fout->fp );


	// 入力ファイルを閉じる
	fclose( infp );

	// 出力ファイルを閉じる
	fclose( fout->fp );

	// DATAブロック領域開放
	for( i=0; i<fout->dbnum; i++ ) free( fdb[i] );

	// P6T情報領域開放
	free( fout->ask );
	free( fout );

	printf_local( "ファイルの変換が完了しました。\n" );

	// パラメータファイルを閉じる
	if ( inparam ) fclose( inparam );
	if ( outparam ) fclose( outparam );

	return 0;
}
