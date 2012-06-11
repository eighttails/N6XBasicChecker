#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "checker.h"

namespace qi    = boost::spirit::qi;
namespace phx   = boost::phoenix;
namespace sw    = qi::standard_wide;

//リテラルパーサー
#define L(a)    lit(L##a)

template <typename Iterator>
bool program_parse(Iterator first, Iterator last, ParserStatus& status)
{
    using sw::char_;
    using qi::_1;
    using qi::int_;
    using qi::uint_;
    using qi::double_;
    using qi::lit;
    using phx::ref;

    typedef qi::rule<Iterator, unsigned()> UintRule;
    typedef qi::rule<Iterator, int()> IntRule;
    typedef qi::rule<Iterator, float()> FloatRule;
    typedef qi::rule<Iterator, sw::blank_type> StringRule;

    //グラフィック文字
    StringRule graph
            =   L("月")|L("火")|L("水")|L("木")|L("金")|L("土")|L("日")|L("年")|L("円")
            |	L("時")|L("分")|L("秒")|L("百")|L("千")|L("万")|L("π")
            |	L("┻")|L("┳")|L("┫")|L("┣")|L("╋")|L("┃")|L("━")|L("┏")|L("┓")|L("┗")|L("┛")
            |	L("×")|L("大")|L("中")|L("小")|L("▲")|L("▼")|L("★")|L("◆")|L("○")|L("●");

    //かな記号
    StringRule kana_kigou
            =	L("「")|L("」")|L("、")|L("・")|L("゛")|L("゜");

    //ひらがな
    StringRule hiragana
            =	L("を")|L("ぁ")|L("ぃ")|L("ぅ")|L("ぇ")|L("ぉ")|L("ゃ")|L("ゅ")|L("ょ")|L("っ")
            |	L("あ")|L("い")|L("う")|L("え")|L("お")|L("か")|L("き")|L("く")|L("け")|L("こ")
            |	L("さ")|L("し")|L("す")|L("せ")|L("そ")|L("た")|L("ち")|L("つ")|L("て")|L("と")
            |	L("な")|L("に")|L("ぬ")|L("ね")|L("の")|L("は")|L("ひ")|L("ふ")|L("へ")|L("ほ")
            |	L("ま")|L("み")|L("む")|L("め")|L("も")|L("や")|L("ゆ")|L("よ")
            |	L("ら")|L("り")|L("る")|L("れ")|L("ろ")|L("わ")|L("ん")
            |	L("が")|L("ぎ")|L("ぐ")|L("げ")|L("ご")|L("ざ")|L("じ")|L("ず")|L("ぜ")|L("ぞ")
            |	L("だ")|L("ぢ")|L("づ")|L("で")|L("ど")|L("ば")|L("び")|L("ぶ")|L("べ")|L("ぼ")
            |	L("ぱ")|L("ぴ")|L("ぷ")|L("ぺ")|L("ぽ");

    //カタカナ
    StringRule katakana
            =	L("ヲ")|L("ァ")|L("ィ")|L("ゥ")|L("ェ")|L("ォ")|L("ャ")|L("ュ")|L("ョ")|L("ッ")
            |	L("ア")|L("イ")|L("ウ")|L("エ")|L("オ")|L("カ")|L("キ")|L("ク")|L("ケ")|L("コ")
            |	L("サ")|L("シ")|L("ス")|L("セ")|L("ソ")|L("タ")|L("チ")|L("ツ")|L("テ")|L("ト")
            |	L("ナ")|L("ニ")|L("ヌ")|L("ネ")|L("ノ")|L("ハ")|L("ヒ")|L("フ")|L("ヘ")|L("ホ")
            |	L("マ")|L("ミ")|L("ム")|L("メ")|L("モ")|L("ヤ")|L("ユ")|L("ヨ")
            |	L("ラ")|L("リ")|L("ル")|L("レ")|L("ロ")|L("ワ")|L("ン")
            |	L("ガ")|L("ギ")|L("グ")|L("ゲ")|L("ゴ")|L("ザ")|L("ジ")|L("ズ")|L("ゼ")|L("ゾ")
            |	L("ダ")|L("ヂ")|L("ヅ")|L("デ")|L("ド")|L("バ")|L("ビ")|L("ブ")|L("ベ")|L("ボ")
            |	L("パ")|L("ピ")|L("プ")|L("ペ")|L("ポ")|L("ヴ");

    //半角カナ
    StringRule han_kana
            =	L("ｦ")|L("ｧ")|L("ｨ")|L("ｩ")|L("ｪ")|L("ｫ")|L("ｬ")|L("ｭ")|L("ｮ")|L("ｯ")
            |	L("ｱ")|L("ｲ")|L("ｳ")|L("ｴ")|L("ｵ")|L("ｶ")|L("ｷ")|L("ｸ")|L("ｹ")|L("ｺ")
            |	L("ｻ")|L("ｼ")|L("ｽ")|L("ｾ")|L("ｿ")|L("ﾀ")|L("ﾁ")|L("ﾂ")|L("ﾃ")|L("ﾄ")
            |	L("ﾅ")|L("ﾆ")|L("ﾇ")|L("ﾈ")|L("ﾉ")|L("ﾊ")|L("ﾋ")|L("ﾌ")|L("ﾍ")|L("ﾎ")
            |	L("ﾏ")|L("ﾐ")|L("ﾑ")|L("ﾒ")|L("ﾓ")|L("ﾔ")|L("ﾕ")|L("ﾖ")
            |	L("ﾗ")|L("ﾘ")|L("ﾙ")|L("ﾚ")|L("ﾛ")|L("ﾜ")|L("ﾝ")|L("ﾞ")|L("ﾟ");

    //使用可能な文字
    StringRule printable
            =	L(" ")|L("!")|L("\"")|L("#")|L("$")|L("%")|L("&")
            |	L("'")|L("(")|L(")")|L("*")
            |	L("+")|L(",")|L("-")|L(".")|L("/")|L(":")|L(";")
            |	L("<")|L("=")|L(">")|L("?")|L("@")|L("[")|L("\\")
            |	L("]")|L("^")|L("_")|L("{")
            |	L("|")|L("}")|L("~")
            |	sw::alpha
            |	sw::digit
            |	graph|kana_kigou|hiragana|katakana|han_kana;

    //数値関連
    StringRule num_group, num_value, num_func, num_expression;
    //文字列関連
    StringRule str_group, str_value, str_func, str_expression;

    //数値型変数
    StringRule num_var = qi::repeat(1, 5)[sw::alpha];

    //数値型変数(配列)
    //#PENDING DIM分との間の次元数チェック
    StringRule num_array_var = num_var >> L("(") >> num_expression >> *(L(",") >> num_expression) >> L(")");

    //数値リテラル
    StringRule num_literal = double_;

    //数値グループ
    num_group
            =   '(' >> num_expression >> ')';

    //数値
    num_value
            =   num_func
            |   num_literal
            |   num_group
            |   num_array_var
            |   num_var;

    //数値式
    num_expression  = num_value >> *(
                                       (L("+") >> num_value)
                                       |(L("-") >> num_value)
                                       |(L("*") >> num_value)
                                       |(L("/") >> num_value)
                                     );

    //文字列変数
    StringRule str_var = qi::repeat(1, 5)[sw::alpha] >> L("$");

    //文字列変数(配列)
    //#PENDING DIM分との間の次元数チェック
    StringRule str_array_var = str_var >> L("(") >> num_expression >> *(L(",") >> num_expression) >> L(")");

    //文字列リテラル(ダブルクオーテーションを含まない)
    StringRule str_literal = *(printable - L("\""));

    //文字列グループ
    str_group
            =   '(' >> str_expression >> ')';

    //文字列値
    str_value
            =   str_func
            //2つ目のダブルクオートの前に「-」が付いているのは、行末のダブルクオートは省略できるという仕様への対応
            |   (L("\"") >> str_literal >> -L("\""))
            |   str_group
            |   str_array_var
            |   str_var;

    //文字列式
    str_expression  = str_value >> *(('+' >> str_value));


    //数値型関数
    //#PENDING
    //num_func= ...

    //文字列関数
    //#PENDING
    //str_func= ...

    //行番号
    UintRule linenumber = uint_;

    //GOTO文
    //goとtoの間には空白を許容するため、トークンを分ける
    StringRule st_goto = L("go") >> L("to") >> linenumber;

    //PRINT文
    StringRule st_print = (L("print")|L("?")) >> str_expression >> *(L(";") >> str_expression);

    //文
    StringRule statement
            = st_goto
            | st_print;

    //行
    StringRule line = linenumber[ref(status.basicLineNumber_) = _1]
            >> statement
            >> *(+L(":") >> statement);

    bool r = qi::phrase_parse(first, last, line, sw::blank);

    if (!r || first != last) {
        return false;
    }

    return true;
}

Checker::Checker()
{
}

bool Checker::parse(const std::wstring& programList, ParserStatus& stat)
{
    stat = ParserStatus();
    //プログラムを行ごとに分割
    std::vector<std::wstring> list;
    boost::algorithm::split(list, programList, boost::is_any_of(L"\n"));

    bool result = true;
    for(size_t i = 0; i < list.size(); i++){
        stat.inclementLine();
        const std::wstring line = list[i];
        if (line.empty()) continue;
        // 1行の構文解析結果を判定
        std::wstring::const_iterator iter = line.begin(), end = line.end();
        bool r = program_parse(iter, end, stat);
        if(!r){
            stat.errorList_.push_back(ErrorInfo(stat.textLineNumber_, stat.basicLineNumber_, L"構文エラー"));
        }
        result &= r;
    }
    return result;
}
