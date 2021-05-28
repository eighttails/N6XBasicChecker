#ifndef CHECKERCOMMON_H
#define CHECKERCOMMON_H

#include <string>

enum ErrorWarningCode
{
	E_UNKNOWN = 0,              //不明なエラー
	W_UNKNOWN = 0,              //不明な警告
	E_SYNTAX,                   //シンタックスエラー
	E_PART_SYNTAX,              //部分シンタックスエラー
	E_LINE_NOT_FOUND,           //行番号が見つからない
	E_INVALID_LINENUMBER,       //行番号が昇順になっていない、ユニークでない
	E_PLAY,                     //PLAY文エラー
	E_TALK,                     //TALK文エラー
	E_HEX,                      //16進数リテラルエラー
	E_DIGIT,                    //10進数リテラルエラー
	W_UNASSIGNED_VARIABLE,      //代入されていない変数
	W_UNREFERED_VARIABLE,       //参照されていない変数
	W_DUPLICATE_VARIABLE,       //識別名が重複している変数
	W_REDUNDANT_CONTENT,        //GOTO文の後に何か書いてある(実行時エラーにはならない)
};

enum RangeType
{
	R_PLAY,
	R_TALK,
	R_HEX,
	R_DIGIT,
};

//エラーやパーサーの状態管理に使われる行番号情報
struct LineNumberInfo
{
	//テキストファイル内の行番号
	int textLineNumber_;
	//BASICリスト内の行番号
	int basicLineNumber_;

	LineNumberInfo()
		: textLineNumber_(0)
		, basicLineNumber_(-1)
	{}

	LineNumberInfo(int textLineNumber, int basicLineNumber)
		: textLineNumber_(textLineNumber)
		, basicLineNumber_(basicLineNumber)
	{}

	//ソート時はテキスト行番号でソート
	bool operator < (const LineNumberInfo& rhs) const{
		return textLineNumber_ < rhs.textLineNumber_;
	}
};

//プログラム内でGOTO,GOSUB,RESTOREなどから参照されている行番号情報
struct ReferredLineNumber
{
	//参照元行番号
	LineNumberInfo refererLine_;

	//参照先行番号
	int targetLineNumber_;

	ReferredLineNumber()
		: targetLineNumber_(-1)
	{}

	ReferredLineNumber(int textLineNumber, int basicLineNumber, int targetLineNumber)
		: refererLine_(textLineNumber, basicLineNumber)
		, targetLineNumber_(targetLineNumber)
	{}
};

//変数の使われ方
enum VarUsage
{
	VAR_REFER,  //参照
	VAR_ASSIGN, //代入
};

//変数が使われている行番号に関する情報
struct VarLineInfo
{
	//行番号
	LineNumberInfo line_;

	//使われているルール(ステートメント、関数)名
	std::wstring ruleName_;

	VarLineInfo(int textLineNumber, int basicLineNumber, const std::wstring& ruleName)
		: line_(textLineNumber, basicLineNumber)
		, ruleName_(ruleName)
	{}

	//ソート時はテキスト行番号でソート
	bool operator < (const VarLineInfo& rhs) const{
		return line_ < rhs.line_;
	}
};

//プログラム内で使用されている変数情報
struct UsedVar
{
	//変数を参照している行番号
	std::set<VarLineInfo> referingLines_;

	//変数に代入している行番号
	std::set<VarLineInfo> assigningLines_;

	//BASICが識別する名前。変数名の先頭2文字。
	//文字列変数の場合は『$』を含めて3文字まで。
	//配列変数の場合はさらに末尾に『()』を付ける。
	std::wstring identName_;

	//BASICリスト上の完全な変数名。
	//変数名内部の空白は除外する。
	//配列変数の場合はさらに末尾に『()』を付ける。(次元、インデックス情報は含まない)
	std::wstring varName_;
};

struct ErrorInfo
{
	//エラー、警告コード
	ErrorWarningCode code_;

	//行番号情報
	LineNumberInfo line_;

	//エラー内容
	std::wstring info_;

	ErrorInfo()
		: code_(E_UNKNOWN)
	{}

	ErrorInfo(ErrorWarningCode code, int textLineNumber, int basicLineNumber, const std::wstring& info)
		: code_(code)
		, line_(textLineNumber, basicLineNumber)
		, info_(info)
	{}

	//ソート時はテキスト行番号でソート
	bool operator < (const ErrorInfo& rhs) const{
		return line_ < rhs.line_;
	}
};

#endif // CHECKERCOMMON_H
