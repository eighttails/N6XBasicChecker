#ifndef CHECKER_H
#define CHECKER_H

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
        : textLineNumber_(1)
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
    : textLineNumber_(1)
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
    bool parse(const std::wstring& prog, ParserStatus& stat);
};

#endif // CHECKER_H
