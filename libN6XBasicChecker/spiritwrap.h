#ifndef SPIRITWRAP_H
#define SPIRITWRAP_H

//boost::spiritに関連するヘッダーおよびtypedef

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "checker.h"

namespace spirit    = boost::spirit;
namespace qi        = boost::spirit::qi;
namespace phx       = boost::phoenix;

namespace sw        = qi::standard_wide;

using sw::char_;
using qi::_1;
using qi::int_;
using qi::uint_;
using qi::double_;
using qi::hex;
using qi::lit;
using phx::ref;

typedef std::wstring::const_iterator Iterator;
typedef qi::rule<Iterator, unsigned()> UintRule;
typedef qi::rule<Iterator, int()> IntRule;
typedef qi::rule<Iterator, float()> FloatRule;
typedef qi::rule<Iterator, std::wstring(), sw::blank_type> StringRule;
typedef qi::rule<Iterator, std::wstring()> NoSkipStringRule;

//リテラルパーサー
#define L(a)    (sw::string(L##a))

#endif // SPIRITWRAP_H
