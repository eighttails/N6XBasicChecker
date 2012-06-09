#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/format.hpp>

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

    qi::rule<Iterator, unsigned()> linenumber("linenumber");
    qi::rule<Iterator, sw::blank_type>
            programlist("programlist"),
            line("line"),
            statement("statement");
    qi::rule<Iterator, sw::blank_type>
            st_goto("st_goto"),
            st_print("st_print");

    linenumber = uint_;

    st_goto = lit(L"go") >> lit(L"to") >> linenumber;

    statement = st_goto;
            //| st_print;

    line = linenumber[ref(status.basicLineNumber_) = _1]
            >> statement
            >> *(+lit(":") >> statement)
            >> qi::eol[ref(status.textLineNumber_)++];

    programlist = +line;

    bool r = qi::phrase_parse(first, last, programlist, sw::blank);

    if (!r || first != last) {
        return false;
    }

    return true;
}

Checker::Checker()
{
}

bool Checker::parse(const std::wstring& str, ParserStatus& stat)
{
    std::wstring::const_iterator iter = str.begin(), end = str.end();

    bool ret = true;
    if (!program_parse(iter, end, stat)) {
        ret = false;
    }
    return ret;
}
