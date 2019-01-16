#ifndef CHECKER_H
#define CHECKER_H

#include <map>
#include <string>

class ParserStatus;

class Checker
{
public:
    Checker();

    //構文解析
    bool parse(const std::wstring& programList, ParserStatus& stat, bool trace = false);

    static const char* version(){ return "1.7.0"; }
private:
    //全角→半角変換
    void convZenHan(std::wstring& programList);
    void replaceString(std::wstring& str, const std::wstring& from, const std::wstring& to);

    //全角半角変換マップ
    void makeZenHanMap();
    std::map<std::wstring, std::wstring> zenhanMap_;

    //事後チェック(2パス目)
    void afterCheck(ParserStatus& stat);
};

#endif // CHECKER_H
