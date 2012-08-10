#include <algorithm>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include "parserstatus.h"


void ParserStatus::registerLineNumber(int basicLineNumber){
    line_.basicLineNumber_ = basicLineNumber;

    if(basicLineNumberList_.size() > 0 && *basicLineNumberList_.rbegin() >= basicLineNumber){
        errorList_.push_back(ErrorInfo(E_INVALID_LINENUMBER, line_.textLineNumber_, line_.basicLineNumber_, L"行番号が昇順になっていません"));
    }

    if(basicLineNumberList_.count(basicLineNumber)){
        errorList_.push_back(ErrorInfo(E_INVALID_LINENUMBER, line_.textLineNumber_, line_.basicLineNumber_, L"行番号に重複があります"));
    }
    basicLineNumberList_.insert(basicLineNumber);
}


void ParserStatus::registerReferredLineNumber(int targetLineNumber){
    ReferredLineNumberList_.push_back(ReferredLineNumber(line_.textLineNumber_, line_.basicLineNumber_, targetLineNumber));
}


void ParserStatus::warnRedundantContent(const std::wstring& tok){
    //出力の際は大文字に変換partial_parse
    std::wstring workTok = tok;
    transform(workTok.begin (), workTok.end (), workTok.begin (), toupper);
    warningList_.push_back(ErrorInfo(W_REDUNDANT_CONTENT, line_.textLineNumber_, line_.basicLineNumber_,
                                     (boost::wformat(L"行番号の後に余分な記述があります。エラーにはなりませんが、意図した記述ですか？(%1%)") % workTok).str()));
}

void ParserStatus::registerUsedVariable(const std::wstring& fullName, VarUsage usage, bool addArraySuffix, const std::wstring& ruleName)
{
    //#PENDING
    std::wcout<<L"---rule:" << ruleName << "\t\t" << (fullName.empty()? L"!!!":fullName) <<std::endl;
    //配列のインデックスを除去し、『()』に置換する。
    boost::wregex regex(L"\\(.*\\)");
    std::wstring varName = boost::regex_replace(fullName, regex, L"\\(\\)", boost::format_all);
    //配列変数であるが『()』を付けない構文の場合、後から『()』を付加する。
    if(addArraySuffix){
        varName += L"()";
    }

    //変数名を空白を含まない形式にする。
    boost::wregex regex2(L" ");
    varName = boost::regex_replace(varName, regex2, L"", boost::format_all);

    //先頭の2文字を抽出して識別名を生成する


    std::wcout<<L"---fullName:" << fullName << "->varName:" << varName <<std::endl;
    //mapに格納
}
