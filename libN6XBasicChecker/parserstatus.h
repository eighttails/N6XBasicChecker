#ifndef PARSERSTATUS_H
#define PARSERSTATUS_H

#include <set>
#include <map>
#include <vector>

#include "checkercommon.h"

struct ParserStatus
{
    //現在処理中の行番号
    LineNumberInfo line_;

    //BASIC行番号のリスト
    std::set<int> basicLineNumberList_;

    std::vector<ReferredLineNumber> ReferredLineNumberList_;

    //プログラム内で使用されている変数のリスト
    //キーは変数の識別名。(先頭2文字を抽出したもの)
    //同一の識別名に複数の長い変数名が使用された場合、警告を出す。
    std::multimap<std::wstring, UsedVar> usedVariables_;

    //現在の読み込み位置の次に現れる変数が代入か参照かを示すフラグ
    VarUsage varMode_;

    //エラー情報のリスト
    std::vector<ErrorInfo> errorList_;

    //警告情報のリスト
    std::vector<ErrorInfo> warningList_;

    ParserStatus()
        : varMode_(VAR_ASSIGN)
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
    void warnRedundantContent(const std::wstring& tok);

    //使用されている変数を登録する。
    void registerUsedVariable(const std::wstring& fullName, VarUsage usage, bool addArraySuffix, const std::wstring &ruleName);

    //行を1行進める。
    void inclementLine(){
        line_.textLineNumber_++;
        line_.basicLineNumber_ = -1;
    }
};

#endif // PARSERSTATUS_H
