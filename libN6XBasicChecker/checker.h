#ifndef CHECKER_H
#define CHECKER_H

#include <map>
#include <string>
#include <vector>

struct ErrorInfo{
    //テキストファイル内の行番号
    int textLineNumber_;
    //BASICリスト内の行番号
    int basicLineNumber_;
    //エラー内容
    std::wstring info_;

    ErrorInfo()
        : textLineNumber_(0)
        , basicLineNumber_(-1)
        {}

    ErrorInfo(unsigned textLineNumber, int basicLineNumber, std::wstring info)
        : textLineNumber_(textLineNumber)
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

    static const wchar_t* version(){ return L"1.0"; }
private:
    //全角→半角変換
    void convZenHan(std::wstring& programList);
    void replaceString(std::wstring& str, const std::wstring& from, const std::wstring& to);

    //全角半角変換マップ
    void makeZenHanMap();
    std::map<std::wstring, std::wstring> zenhanMap_;
};

#endif // CHECKER_H
