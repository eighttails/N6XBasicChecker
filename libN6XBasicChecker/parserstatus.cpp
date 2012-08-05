#include <algorithm>
#include <boost/format.hpp>

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


void ParserStatus::warnRedundantContent(std::wstring tok){
    //出力の際は大文字に変換partial_parse
    std::wstring workTok = tok;
    transform(workTok.begin (), workTok.end (), workTok.begin (), toupper);
    warningList_.push_back(ErrorInfo(W_REDUNDANT_CONTENT, line_.textLineNumber_, line_.basicLineNumber_,
                                     (boost::wformat(L"行番号の後に余分な記述があります。エラーにはなりませんが、意図した記述ですか？(%1%)") % workTok).str()));
}
