#ifndef PARSERSTATUS_H
#define PARSERSTATUS_H

#include <set>
#include <vector>

#include "checkercommon.h"

struct ParserStatus
{
    //現在処理中の行番号
    LineNumberInfo line_;

    //BASIC行番号のリスト
    std::set<int> basicLineNumberList_;

    std::vector<ReferredLineNumber> ReferredLineNumberList_;

    //エラー情報のリスト
    std::vector<ErrorInfo> errorList_;

    //警告情報のリスト
    std::vector<ErrorInfo> warningList_;

    ParserStatus()
    {}

    //行番号を登録する。
    //行を1行認識するごとに実行する。
    //全行読み込み後にここで登録した行番号の整合性、および
    //GOTOなどで参照された行番号の存在チェックを行う。
    void registerLineNumber(int basicLineNumber);

    //行番号の参照を登録する
    void registerReferredLineNumber(int targetLineNumber);

    //GOTO,GOSUBの後の余分な記述を登録する。
    //実行時エラーにならないので、コメントなどを入れる人がいるが、
    //GOTOの後のコロンが抜けた場合、次のステートメントが実行されない事態になるため、
    //警告を出すために登録する。
    void warnRedundantContent(std::wstring tok);

    //行を1行進める。
    void inclementLine(){
        line_.textLineNumber_++;
        line_.basicLineNumber_ = -1;
    }
};

#endif // PARSERSTATUS_H
