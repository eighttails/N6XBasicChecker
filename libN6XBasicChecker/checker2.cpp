#include "checker.h"
#include "parserstatus.h"

#include "spiritwrap.h"

void Checker::makeZenHanMap()
{
    //全角半角テーブル
    zenhanMap_[L"　"] = L" ";
    zenhanMap_[L"！"] = L"!";
    zenhanMap_[L"”"] = L"\"";
    zenhanMap_[L"＃"] = L"#";
    zenhanMap_[L"＄"] = L"$";
    zenhanMap_[L"％"] = L"%";
    zenhanMap_[L"＆"] = L"&";
    zenhanMap_[L"’"] = L"'";
    zenhanMap_[L"（"] = L"(";
    zenhanMap_[L"）"] = L")";
    zenhanMap_[L"＊"] = L"*";
    zenhanMap_[L"＋"] = L"+";
    zenhanMap_[L"，"] = L",";
    zenhanMap_[L"－"] = L"-";
    zenhanMap_[L"．"] = L".";
    zenhanMap_[L"／"] = L"/";
    zenhanMap_[L"："] = L":";
    zenhanMap_[L"；"] = L";";
    zenhanMap_[L"＜"] = L"<";
    zenhanMap_[L"＝"] = L"=";
    zenhanMap_[L"＞"] = L">";
    zenhanMap_[L"？"] = L"?";
    zenhanMap_[L"＠"] = L"@";
    zenhanMap_[L"〔"] = L"[";
    zenhanMap_[L"［"] = L"[";
    zenhanMap_[L"￥"] = L"\\";
    zenhanMap_[L"〕"] = L"]";
    zenhanMap_[L"］"] = L"]";
    zenhanMap_[L"＾"] = L"^";
    zenhanMap_[L"＿"] = L"_";
    zenhanMap_[L"｛"] = L"{";
    zenhanMap_[L"｜"] = L"|";
    zenhanMap_[L"｝"] = L"}";
    zenhanMap_[L"〜"] = L"~";

    zenhanMap_[L"０"] = L"0";
    zenhanMap_[L"１"] = L"1";
    zenhanMap_[L"２"] = L"2";
    zenhanMap_[L"３"] = L"3";
    zenhanMap_[L"４"] = L"4";
    zenhanMap_[L"５"] = L"5";
    zenhanMap_[L"６"] = L"6";
    zenhanMap_[L"７"] = L"7";
    zenhanMap_[L"８"] = L"8";
    zenhanMap_[L"９"] = L"9";

    zenhanMap_[L"Ａ"] = L"A";
    zenhanMap_[L"ａ"] = L"a";
    zenhanMap_[L"Ｂ"] = L"B";
    zenhanMap_[L"ｂ"] = L"b";
    zenhanMap_[L"Ｃ"] = L"C";
    zenhanMap_[L"ｃ"] = L"c";
    zenhanMap_[L"Ｄ"] = L"D";
    zenhanMap_[L"ｄ"] = L"d";
    zenhanMap_[L"Ｅ"] = L"E";
    zenhanMap_[L"ｅ"] = L"e";
    zenhanMap_[L"Ｆ"] = L"F";
    zenhanMap_[L"ｆ"] = L"f";
    zenhanMap_[L"Ｇ"] = L"G";
    zenhanMap_[L"ｇ"] = L"g";
    zenhanMap_[L"Ｈ"] = L"H";
    zenhanMap_[L"ｈ"] = L"h";
    zenhanMap_[L"Ｉ"] = L"I";
    zenhanMap_[L"ｉ"] = L"i";
    zenhanMap_[L"Ｊ"] = L"J";
    zenhanMap_[L"ｊ"] = L"j";
    zenhanMap_[L"Ｋ"] = L"K";
    zenhanMap_[L"ｋ"] = L"k";
    zenhanMap_[L"Ｌ"] = L"L";
    zenhanMap_[L"ｌ"] = L"l";
    zenhanMap_[L"Ｍ"] = L"M";
    zenhanMap_[L"ｍ"] = L"m";
    zenhanMap_[L"Ｎ"] = L"N";
    zenhanMap_[L"ｎ"] = L"n";
    zenhanMap_[L"Ｏ"] = L"O";
    zenhanMap_[L"ｏ"] = L"o";
    zenhanMap_[L"Ｐ"] = L"P";
    zenhanMap_[L"ｐ"] = L"p";
    zenhanMap_[L"Ｑ"] = L"Q";
    zenhanMap_[L"ｑ"] = L"q";
    zenhanMap_[L"Ｒ"] = L"R";
    zenhanMap_[L"ｒ"] = L"r";
    zenhanMap_[L"Ｓ"] = L"S";
    zenhanMap_[L"ｓ"] = L"s";
    zenhanMap_[L"Ｔ"] = L"T";
    zenhanMap_[L"ｔ"] = L"t";
    zenhanMap_[L"Ｕ"] = L"U";
    zenhanMap_[L"ｕ"] = L"u";
    zenhanMap_[L"Ｖ"] = L"V";
    zenhanMap_[L"ｖ"] = L"v";
    zenhanMap_[L"Ｗ"] = L"W";
    zenhanMap_[L"ｗ"] = L"w";
    zenhanMap_[L"Ｘ"] = L"X";
    zenhanMap_[L"ｘ"] = L"x";
    zenhanMap_[L"Ｙ"] = L"Y";
    zenhanMap_[L"ｙ"] = L"y";
    zenhanMap_[L"Ｚ"] = L"Z";
    zenhanMap_[L"ｚ"] = L"z";

    zenhanMap_[L"\r\n"] = L"\n";

}

void Checker::replaceString( std::wstring& str, const std::wstring& from, const std::wstring& to )
{
    std::wstring::size_type  p( str.find( from ) );

    while( p != std::wstring::npos )
    {
        str.replace( p, from.length(), to );
        p = str.find( from, p + to.length() );
    }
}

void Checker::convZenHan(std::wstring& programList)
{
    //全角半角テーブルに従って全角文字を半角に置換
    std::map<std::wstring, std::wstring>::const_iterator p;
    for (p = zenhanMap_.begin(); p != zenhanMap_.end(); ++p){
        replaceString(programList, p->first, p->second);
    }
}

void Checker::afterCheck(ParserStatus& stat)
{
    //行番号チェック
    for (size_t i = 0; i < stat.ReferredLineNumberList_.size(); i++){
        const ReferredLineNumber& ref = stat.ReferredLineNumberList_[i];
        if(stat.basicLineNumberList_.count(ref.targetLineNumber_) == 0){
            stat.errorList_.push_back(ErrorInfo(E_LINE_NOT_FOUND, ref.refererLine_.textLineNumber_, ref.refererLine_.basicLineNumber_,
                                                (boost::wformat(L"BASIC行%1%は存在しません") % ref.targetLineNumber_).str()));
        }
    }

    //変数チェック
    std::map<std::wstring, std::map<std::wstring, UsedVar> >::const_iterator i;
    std::map<std::wstring, UsedVar>::const_iterator j;
    std::set<LineNumberInfo>::const_iterator k;

    for(i = stat.usedVariables_.begin(); i != stat.usedVariables_.end(); ++i){
        const std::map<std::wstring, UsedVar>& subMap = (*i).second;
        for(j = subMap.begin(); j != subMap.end(); ++j){
            const UsedVar& var = (*j).second;
            //識別子重複チェック(2文字目までが一致しているが、違う名前が使われている変数)
            if(subMap.size() > 1){
                for(k = var.assigningLines_.begin(); k != var.assigningLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).textLineNumber_, (*k).basicLineNumber_,
                                                          (boost::wformat(L"[%1%]として識別される変数が複数存在します[%2%]") % var.identName_ % var.varName_).str()));
                }
                for(k = var.referingLines_.begin(); k != var.referingLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).textLineNumber_, (*k).basicLineNumber_,
                                                          (boost::wformat(L"[%1%]として識別される変数が複数存在します[%2%]") % var.identName_ % var.varName_).str()));
                }
            }

            //参照されているが代入されていない変数
            if(var.assigningLines_.empty()){
                for(k = var.referingLines_.begin(); k != var.referingLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).textLineNumber_, (*k).basicLineNumber_,
                                                          (boost::wformat(L"変数[%1%]はどこからも代入されていません") % var.varName_).str()));
                }
            }

            //代入されているが参照されていない変数
            if(var.referingLines_.empty()){
                for(k = var.assigningLines_.begin(); k != var.assigningLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).textLineNumber_, (*k).basicLineNumber_,
                                                          (boost::wformat(L"変数[%1%]はどこからも参照されていません") % var.varName_).str()));
                }
            }
        }
    }
}
