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

struct ErrorInfo
{
    //テキストファイル内の行番号
    int textLineNumber_;
    //BASICリスト内の行番号
    int basicLineNumber_;
    //エラー、警告コード
    ErrorWarningCode code_;
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
