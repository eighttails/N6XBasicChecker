#ifndef CHECKER_H
#define CHECKER_H

#include <string>

struct ParserStatus
{
    int textLineNumber_;
    int basicLineNumber_;
    ParserStatus()
    : textLineNumber_(1)
    , basicLineNumber_(0)
    {}
};

class Checker
{
public:
    Checker();
    bool parse(const std::wstring& prog, ParserStatus& stat);
};

#endif // CHECKER_H
