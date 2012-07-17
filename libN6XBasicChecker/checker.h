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
    std::string info_;

    ErrorInfo()
        : textLineNumber_(0)
        , basicLineNumber_(-1)
        {}

    ErrorInfo(unsigned textLineNumber, int basicLineNumber, std::string info)
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
    bool parse(const std::string& programList, ParserStatus& stat, bool trace = false);

    static const char* version(){ return "1.0"; }
private:
    //全角→半角変換
    void convZenHan(std::string& programList);
    void replaceString(std::string& str, const std::string& from, std::string to);

    //全角半角変換マップ
    void makeZenHanMap();
    std::map<std::string, std::string> zenhanMap_;
};

#endif // CHECKER_H
