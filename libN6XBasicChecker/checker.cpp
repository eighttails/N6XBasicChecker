#include "checker.h"

#include "pch.h"

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

    linenumber = uint_;

    statement = lit(L"go") >> lit(L"to") >> linenumber;

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
    stat = ParserStatus();
    std::wstring::const_iterator iter = str.begin(), end = str.end();

    bool ret = true;
    if (!program_parse(iter, end, stat)) {
        std::cout << "failed" << std::endl;
        ret = false;
    }else {
        std::cout << "success" << std::endl;
    }
    return ret;
}
