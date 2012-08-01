#ifndef CHECKER_H
#define CHECKER_H

#include <map>
#include <set>
#include <string>
#include <vector>

enum ErrorWarningCode
{
    E_UNKNOWN = 0,              //不明なエラー
    W_UNKNOWN = 0,              //不明な警告
    E_SYNTAX,                   //シンタックスエラー
    E_PART_SYNTAX,              //部分シンタックスエラー
    E_LINE_NOT_FOUND,           //行番号が見つからない
    E_INVALID_LINENUMBER,       //行番号が昇順になっていない、ユニークでない
    E_PLAY,                     //PLAY文エラー
    E_TALK,                     //TALK文エラー
    E_HEX,                      //16進数リテラルエラー
    W_UNASSIGNED_VARIABLE,      //代入されていない変数
    W_UNUSED_VARIABLE,          //参照されていない変数
    W_DUPLICATE_VARIABLE,       //識別名が重複している変数
};

//エラーやパーサーの状態管理に使われる行番号情報
struct LineNumberInfo
{
    //テキストファイル内の行番号
    int textLineNumber_;
    //BASICリスト内の行番号
    int basicLineNumber_;

    LineNumberInfo()
        : textLineNumber_(0)
        , basicLineNumber_(-1)
    {}

    LineNumberInfo(int textLineNumber, int basicLineNumber)
        : textLineNumber_(textLineNumber)
        , basicLineNumber_(basicLineNumber)
    {}
};

//プログラム内でGOTO,GOSUB,RESTOREなどから参照されている行番号情報
struct ReferredLineNumber
{
    //参照元行番号
    LineNumberInfo refererLine_;

    //参照先行番号
    int targetLineNumber_;

    ReferredLineNumber()
        : targetLineNumber_(-1)
    {}
};

//プログラム内で使用されている変数情報
struct UsedVar
{
    //行番号
    LineNumberInfo line_;

    //BASICが識別する名前。変数名の先頭2文字。
    //文字列変数の場合は『$』を含めて3文字まで。
    //配列変数の場合はさらに末尾に『()』を付ける。
    std::wstring identName_;

    //BASICリスト上の完全な変数名。
    //変数名内部の空白は除外する。
    //配列変数の場合はさらに末尾に『()』を付ける。(次元、インデックス情報は含まない)
    std::wstring fullName_;
};

struct ErrorInfo
{
    //エラー、警告コード
    ErrorWarningCode code_;

    //行番号情報
    LineNumberInfo line_;

    //エラー内容
    std::wstring info_;

    ErrorInfo()
        : code_(E_UNKNOWN)
    {}

    ErrorInfo(ErrorWarningCode code, int textLineNumber, int basicLineNumber, std::wstring info)
        : code_(code)
        , line_(textLineNumber, basicLineNumber)
        , info_(info)
    {}
};


struct ParserStatus
{
    //現在処理中の行番号
    LineNumberInfo line_;

    //BASIC行番号のリスト
    std::set<int> basicLineNumberList_;

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
    void registerLineNumber(int basicLineNumber){
        line_.basicLineNumber_ = basicLineNumber;

        if(basicLineNumberList_.size() > 0 && *basicLineNumberList_.rbegin() >= basicLineNumber){
            errorList_.push_back(ErrorInfo(E_INVALID_LINENUMBER, line_.textLineNumber_, line_.basicLineNumber_, L"行番号が昇順になっていません"));
        }

        if(basicLineNumberList_.count(basicLineNumber)){
            errorList_.push_back(ErrorInfo(E_INVALID_LINENUMBER, line_.textLineNumber_, line_.basicLineNumber_, L"行番号に重複があります"));
        }
        basicLineNumberList_.insert(basicLineNumber);
    }

    //行を1行進める。
    void inclementLine(){
        line_.textLineNumber_++;
        line_.basicLineNumber_ = -1;
    }
};

class Checker
{
public:
    Checker();
    //構文解析
    bool parse(const std::wstring& programList, ParserStatus& stat, bool trace = false);

    static const char* version(){ return "1.01"; }
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
