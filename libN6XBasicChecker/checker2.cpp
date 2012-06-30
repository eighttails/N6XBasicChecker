#include "checker.h"

void Checker::makeZenHanMap()
{
    //全角半角テーブル
    zenhanMap_["　"] = " ";
    zenhanMap_["！"] = "!";
    zenhanMap_["”"] = "\"";
    zenhanMap_["＃"] = "#";
    zenhanMap_["＄"] = "$";
    zenhanMap_["％"] = "%";
    zenhanMap_["＆"] = "&";
    zenhanMap_["’"] = "'";
    zenhanMap_["（"] = "(";
    zenhanMap_["）"] = ")";
    zenhanMap_["＊"] = "*";
    zenhanMap_["＋"] = "+";
    zenhanMap_["，"] = ",";
    zenhanMap_["－"] = "-";
    zenhanMap_["．"] = ".";
    zenhanMap_["／"] = "/";
    zenhanMap_["："] = ":";
    zenhanMap_["；"] = ";";
    zenhanMap_["＜"] = "<";
    zenhanMap_["＝"] = "=";
    zenhanMap_["＞"] = ">";
    zenhanMap_["？"] = "?";
    zenhanMap_["＠"] = "@";
    zenhanMap_["〔"] = "[";
    zenhanMap_["［"] = "[";
    zenhanMap_["￥"] = "\\";
    zenhanMap_["〕"] = "]";
    zenhanMap_["］"] = "]";
    zenhanMap_["＾"] = "^";
    zenhanMap_["＿"] = "_";
    zenhanMap_["｛"] = "{";
    zenhanMap_["｜"] = "|";
    zenhanMap_["｝"] = "}";
    zenhanMap_["〜"] = "~";

    zenhanMap_["０"] = "0";
    zenhanMap_["１"] = "1";
    zenhanMap_["２"] = "2";
    zenhanMap_["３"] = "3";
    zenhanMap_["４"] = "4";
    zenhanMap_["５"] = "5";
    zenhanMap_["６"] = "6";
    zenhanMap_["７"] = "7";
    zenhanMap_["８"] = "8";
    zenhanMap_["９"] = "9";

    zenhanMap_["Ａ"] = "A";
    zenhanMap_["ａ"] = "a";
    zenhanMap_["Ｂ"] = "B";
    zenhanMap_["ｂ"] = "b";
    zenhanMap_["Ｃ"] = "C";
    zenhanMap_["ｃ"] = "c";
    zenhanMap_["Ｄ"] = "D";
    zenhanMap_["ｄ"] = "d";
    zenhanMap_["Ｅ"] = "E";
    zenhanMap_["ｅ"] = "e";
    zenhanMap_["Ｆ"] = "F";
    zenhanMap_["ｆ"] = "f";
    zenhanMap_["Ｇ"] = "G";
    zenhanMap_["ｇ"] = "g";
    zenhanMap_["Ｈ"] = "H";
    zenhanMap_["ｈ"] = "h";
    zenhanMap_["Ｉ"] = "I";
    zenhanMap_["ｉ"] = "i";
    zenhanMap_["Ｊ"] = "J";
    zenhanMap_["ｊ"] = "j";
    zenhanMap_["Ｋ"] = "K";
    zenhanMap_["ｋ"] = "k";
    zenhanMap_["Ｌ"] = "L";
    zenhanMap_["ｌ"] = "l";
    zenhanMap_["Ｍ"] = "M";
    zenhanMap_["ｍ"] = "m";
    zenhanMap_["Ｎ"] = "N";
    zenhanMap_["ｎ"] = "n";
    zenhanMap_["Ｏ"] = "O";
    zenhanMap_["ｏ"] = "o";
    zenhanMap_["Ｐ"] = "P";
    zenhanMap_["ｐ"] = "p";
    zenhanMap_["Ｑ"] = "Q";
    zenhanMap_["ｑ"] = "q";
    zenhanMap_["Ｒ"] = "R";
    zenhanMap_["ｒ"] = "r";
    zenhanMap_["Ｓ"] = "S";
    zenhanMap_["ｓ"] = "s";
    zenhanMap_["Ｔ"] = "T";
    zenhanMap_["ｔ"] = "t";
    zenhanMap_["Ｕ"] = "U";
    zenhanMap_["ｕ"] = "u";
    zenhanMap_["Ｖ"] = "V";
    zenhanMap_["ｖ"] = "v";
    zenhanMap_["Ｗ"] = "W";
    zenhanMap_["ｗ"] = "w";
    zenhanMap_["Ｘ"] = "X";
    zenhanMap_["ｘ"] = "x";
    zenhanMap_["Ｙ"] = "Y";
    zenhanMap_["ｙ"] = "y";
    zenhanMap_["Ｚ"] = "Z";
    zenhanMap_["ｚ"] = "z";

    zenhanMap_["\r\n"] = "\n";

}

void Checker::replaceString( std::string& str, const std::string& from, std::string to )
{
    std::string::size_type  p( str.find( from ) );

    while( p != std::string::npos )
    {
        str.replace( p, from.length(), to );
        p = str.find( from, p + to.length() );
    }
}

void Checker::convZenHan(std::string& programList)
{
    //全角半角テーブルに従って全角文字を半角に置換
    std::map<std::string, std::string>::const_iterator p;
    for (p = zenhanMap_.begin(); p != zenhanMap_.end(); ++p){
        replaceString(programList, p->first, p->second);
    }
}
