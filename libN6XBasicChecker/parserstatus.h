#ifndef PARSERSTATUS_H
#define PARSERSTATUS_H

#include <set>
#include <map>
#include <vector>

#include "checkercommon.h"

struct ParserStatus
{
    // 現在処理中の行番号
    LineNumberInfo line_;

    // BASIC行番号のリスト
    std::set<int> basicLineNumberList_;

    std::vector<ReferredLineNumber> ReferredLineNumberList_;

    // プログラム内で使用されている変数のリスト
    // 第1のキーは変数の識別名。(先頭2文字を抽出したもの)
    // 第1のキーは変数の完全名。(変数名5文字まで+文字列の場合『$』、配列の場合『()』が続く)
    // 同一の識別名に複数の長い変数名が使用された場合、警告を出す。
    std::map<std::wstring, std::map<std::wstring, UsedVar> > usedVariables_;

    // 現在の読み込み位置の次に現れる変数が代入か参照かを示すフラグ
    VarUsage varMode_;

    // 現在処理中のステートメントがPLAY文、またはTALK文であるかのフラグ
    bool playMode_;
    bool talkMode_;

    typedef std::vector<std::pair<int, int> > LineRange;
    // PLAY文として解釈する行番号範囲のリスト
    // ここで指定された行番号の範囲内にある文字列リテラルはPLAY文とみなしてチェックする。

    LineRange playRange_;

    // TALK文として解釈する行番号範囲のリスト
    // ここで指定された行番号の範囲内にある文字列リテラルはTALK文とみなしてチェックする。
    LineRange talkRange_;

    // 16進数として解釈する行番号範囲のリスト
    // ここで指定された行番号の範囲内にある文字列リテラルは16進数とみなしてチェックする。
    LineRange hexRange_;

    // 10進数として解釈する行番号範囲のリスト
    // ここで指定された行番号の範囲内にある文字列リテラルは10進数の符号付き整数とみなしてチェックする。
    LineRange digitRange_;

    // エラー情報のリスト
    std::vector<ErrorInfo> errorList_;

    // 警告情報のリスト
    std::vector<ErrorInfo> warningList_;

    ParserStatus()
        : varMode_(VAR_ASSIGN)
        , playMode_(false)
        , talkMode_(false)
    {}

    // 行番号を登録する。
    // 行を1行認識するごとに実行する。
    // 全行読み込み後にここで登録した行番号の整合性、および
    // GOTOなどで参照された行番号の存在チェックを行う。
    void registerLineNumber(int basicLineNumber);

    // 行番号の参照を登録する
    void registerReferredLineNumber(int targetLineNumber);

    // GOTO,GOSUBの後の余分な記述を登録する。
    // 実行時エラーにならないので、コメントなどを入れる人がいるが、
    // GOTOの後のコロンが抜けた場合、次のステートメントが実行されない事態になるため、
    // 警告を出すために登録する。
    void warnRedundantContent(const std::wstring& tok);

    // 使用されている変数を登録する。
    void registerUsedVariable(const std::wstring& fullName, VarUsage usage, bool addArraySuffix, const std::wstring &ruleName);

    // PLAY文、TALK文としてチェックする行番号範囲を指定する。
    bool registerLineRange(const std::wstring& lines, RangeType type);
    void registerLineRangeInt(int start, int end, RangeType type);

    // 行を1行進める。
    void inclementLine(){
        line_.textLineNumber_++;
        line_.basicLineNumber_ = -1;
    }
};

#endif // PARSERSTATUS_H
