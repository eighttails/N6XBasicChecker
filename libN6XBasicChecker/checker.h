#ifndef CHECKER_H
#define CHECKER_H

#include <map>
#include <string>
#include <vector>

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
    W_UNINITIALIZED_VARIABLE,   //代入されていない変数
    W_UNUSED_VARIABLE,          //参照されていない変数
};

//プログラム内でGOTO,GOSUB,RESTOREなどから参照されている行番号情報
struct ReferredLineNumber
{
    //参照元行番号
    int basicLineNumber_;
    //参照先行番号
    int targetLineNumber_;

    ReferredLineNumber()
        : basicLineNumber_(-1)
        , targetLineNumber_(-1)
    {}
};

//プログラム内で使用されている変数情報
struct UsedVar
{
    //行番号
    int basicLineNumber_;

    //BASICが識別する名前。変数名の先頭2文字。
    //文字列変数の場合は『$』を含めて3文字まで。
    //配列変数の場合はさらに末尾に『()』を付ける。
    std::wstring identName_;

    //BASICリスト上の完全な変数名。
    //変数名内部の空白は除外する。
    //配列変数の場合はさらに末尾に『()』を付ける。(次元、インデックス情報は含まない)
    std::wstring fullName_;

    UsedVar()
        : basicLineNumber_(-1)
    {}
};

struct ErrorInfo
{
    //エラー、警告コード
    ErrorWarningCode code_;
    //テキストファイル内の行番号
    int textLineNumber_;
    //BASICリスト内の行番号
    int basicLineNumber_;
    //エラー内容
    std::wstring info_;

    ErrorInfo()
        : code_(E_UNKNOWN)
        , textLineNumber_(0)
        , basicLineNumber_(-1)
    {}

    ErrorInfo(ErrorWarningCode code, unsigned textLineNumber, int basicLineNumber, std::wstring info)
        : code_(code)
        , textLineNumber_(textLineNumber)
        , basicLineNumber_(basicLineNumber)
        , info_(info)
    {}
};


struct ParserStatus
{
    //テキストファイル内の行番号
    int textLineNumber_;
    //BASICリスト内の行番号
    int basicLineNumber_;

    //BASIC行番号のリスト
    std::vector<int> basicLineNumberList_;

    //エラー情報のリスト
    std::vector<ErrorInfo> errorList_;

    //警告情報のリスト
    std::vector<ErrorInfo> warningList_;

    ParserStatus()
    : textLineNumber_(0)
    , basicLineNumber_(-1)
    {}

    //行を1行進める。
    void inclementLine(){
        textLineNumber_++;
        basicLineNumber_ = -1;
    }

};

class Checker
{
public:
    Checker();
    //構文解析
    bool parse(const std::wstring& programList, ParserStatus& stat, bool trace = false);

    static const char* version(){ return "1.01"; }
private:
    //全角→半角変換
    void convZenHan(std::wstring& programList);
    void replaceString(std::wstring& str, const std::wstring& from, const std::wstring& to);

    //全角半角変換マップ
    void makeZenHanMap();
    std::map<std::wstring, std::wstring> zenhanMap_;
};

#endif // CHECKER_H
