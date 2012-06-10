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
    typedef qi::rule<Iterator, std::wstring(), sw::blank_type> StringRule;

    //グラフィック文字
    StringRule graph
            =    lit(L"月")|lit(L"火")|lit(L"水")|lit(L"木")|lit(L"金")|lit(L"土")|lit(L"日")|lit(L"年")|lit(L"円")
            |	lit(L"時")|lit(L"分")|lit(L"秒")|lit(L"百")|lit(L"千")|lit(L"万")|lit(L"π")
            |	lit(L"┻")|lit(L"┳")|lit(L"┫")|lit(L"┣")|lit(L"╋")|lit(L"┃")|lit(L"━")|lit(L"┏")|lit(L"┓")|lit(L"┗")|lit(L"┛")
            |	lit(L"×")|lit(L"大")|lit(L"中")|lit(L"小")|lit(L"▲")|lit(L"▼")|lit(L"★")|lit(L"◆")|lit(L"○")|lit(L"●");
    //かな記号
    StringRule kana_kigou
            =	lit(L"「")|lit(L"」")|lit(L"、")|lit(L"・")|lit(L"゛")|lit(L"゜");

    //ひらがな
    StringRule hiragana
            =	lit(L"を")|lit(L"ぁ")|lit(L"ぃ")|lit(L"ぅ")|lit(L"ぇ")|lit(L"ぉ")|lit(L"ゃ")|lit(L"ゅ")|lit(L"ょ")|lit(L"っ")
            |	lit(L"あ")|lit(L"い")|lit(L"う")|lit(L"え")|lit(L"お")|lit(L"か")|lit(L"き")|lit(L"く")|lit(L"け")|lit(L"こ")
            |	lit(L"さ")|lit(L"し")|lit(L"す")|lit(L"せ")|lit(L"そ")|lit(L"た")|lit(L"ち")|lit(L"つ")|lit(L"て")|lit(L"と")
            |	lit(L"な")|lit(L"に")|lit(L"ぬ")|lit(L"ね")|lit(L"の")|lit(L"は")|lit(L"ひ")|lit(L"ふ")|lit(L"へ")|lit(L"ほ")
            |	lit(L"ま")|lit(L"み")|lit(L"む")|lit(L"め")|lit(L"も")|lit(L"や")|lit(L"ゆ")|lit(L"よ")
            |	lit(L"ら")|lit(L"り")|lit(L"る")|lit(L"れ")|lit(L"ろ")|lit(L"わ")|lit(L"ん")
            |	lit(L"が")|lit(L"ぎ")|lit(L"ぐ")|lit(L"げ")|lit(L"ご")|lit(L"ざ")|lit(L"じ")|lit(L"ず")|lit(L"ぜ")|lit(L"ぞ")
            |	lit(L"だ")|lit(L"ぢ")|lit(L"づ")|lit(L"で")|lit(L"ど")|lit(L"ば")|lit(L"び")|lit(L"ぶ")|lit(L"べ")|lit(L"ぼ")
            |	lit(L"ぱ")|lit(L"ぴ")|lit(L"ぷ")|lit(L"ぺ")|lit(L"ぽ");

    //カタカナ
    StringRule katakana
            =	lit(L"ヲ")|lit(L"ァ")|lit(L"ィ")|lit(L"ゥ")|lit(L"ェ")|lit(L"ォ")|lit(L"ャ")|lit(L"ュ")|lit(L"ョ")|lit(L"ッ")
            |	lit(L"ア")|lit(L"イ")|lit(L"ウ")|lit(L"エ")|lit(L"オ")|lit(L"カ")|lit(L"キ")|lit(L"ク")|lit(L"ケ")|lit(L"コ")
            |	lit(L"サ")|lit(L"シ")|lit(L"ス")|lit(L"セ")|lit(L"ソ")|lit(L"タ")|lit(L"チ")|lit(L"ツ")|lit(L"テ")|lit(L"ト")
            |	lit(L"ナ")|lit(L"ニ")|lit(L"ヌ")|lit(L"ネ")|lit(L"ノ")|lit(L"ハ")|lit(L"ヒ")|lit(L"フ")|lit(L"ヘ")|lit(L"ホ")
            |	lit(L"マ")|lit(L"ミ")|lit(L"ム")|lit(L"メ")|lit(L"モ")|lit(L"ヤ")|lit(L"ユ")|lit(L"ヨ")
            |	lit(L"ラ")|lit(L"リ")|lit(L"ル")|lit(L"レ")|lit(L"ロ")|lit(L"ワ")|lit(L"ン")
            |	lit(L"ガ")|lit(L"ギ")|lit(L"グ")|lit(L"ゲ")|lit(L"ゴ")|lit(L"ザ")|lit(L"ジ")|lit(L"ズ")|lit(L"ゼ")|lit(L"ゾ")
            |	lit(L"ダ")|lit(L"ヂ")|lit(L"ヅ")|lit(L"デ")|lit(L"ド")|lit(L"バ")|lit(L"ビ")|lit(L"ブ")|lit(L"ベ")|lit(L"ボ")
            |	lit(L"パ")|lit(L"ピ")|lit(L"プ")|lit(L"ペ")|lit(L"ポ")|lit(L"ヴ");
    //半角カナ
    StringRule han_kana
            =	lit(L"ｦ")|lit(L"ｧ")|lit(L"ｨ")|lit(L"ｩ")|lit(L"ｪ")|lit(L"ｫ")|lit(L"ｬ")|lit(L"ｭ")|lit(L"ｮ")|lit(L"ｯ")
            |	lit(L"ｱ")|lit(L"ｲ")|lit(L"ｳ")|lit(L"ｴ")|lit(L"ｵ")|lit(L"ｶ")|lit(L"ｷ")|lit(L"ｸ")|lit(L"ｹ")|lit(L"ｺ")
            |	lit(L"ｻ")|lit(L"ｼ")|lit(L"ｽ")|lit(L"ｾ")|lit(L"ｿ")|lit(L"ﾀ")|lit(L"ﾁ")|lit(L"ﾂ")|lit(L"ﾃ")|lit(L"ﾄ")
            |	lit(L"ﾅ")|lit(L"ﾆ")|lit(L"ﾇ")|lit(L"ﾈ")|lit(L"ﾉ")|lit(L"ﾊ")|lit(L"ﾋ")|lit(L"ﾌ")|lit(L"ﾍ")|lit(L"ﾎ")
            |	lit(L"ﾏ")|lit(L"ﾐ")|lit(L"ﾑ")|lit(L"ﾒ")|lit(L"ﾓ")|lit(L"ﾔ")|lit(L"ﾕ")|lit(L"ﾖ")
            |	lit(L"ﾗ")|lit(L"ﾘ")|lit(L"ﾙ")|lit(L"ﾚ")|lit(L"ﾛ")|lit(L"ﾜ")|lit(L"ﾝ")|lit(L"ﾞ")|lit(L"ﾟ");

    //行番号
    UintRule linenumber = uint_;

    //GOTO文
    StringRule st_goto = lit(L"go") >> lit(L"to") >> linenumber;

    //文
    StringRule statement = st_goto;
    //| st_print;

    //行
    StringRule line = linenumber[ref(status.basicLineNumber_) = _1]
            >> statement
            >> *(+lit(":") >> statement);

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
