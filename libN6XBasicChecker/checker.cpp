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


    UintRule linenumber = uint_;

    StringRule st_goto = lit(L"go") >> lit(L"to") >> linenumber;

    StringRule statement = st_goto;
            //| st_print;

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
