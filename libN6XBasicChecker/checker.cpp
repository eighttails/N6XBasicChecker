#include <iostream>
#include <string>
#include <algorithm>

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

typedef std::string::const_iterator Iterator;
typedef qi::rule<Iterator, unsigned()> UintRule;
typedef qi::rule<Iterator, int()> IntRule;
typedef qi::rule<Iterator, float()> FloatRule;
typedef qi::rule<Iterator, sw::blank_type> StringRule;

//リテラルパーサー
#define L(a)    (sw::string(a))

//部分パーサー
//Spiritのルールは通常先頭から最長一致でマッチしてしまうため、
//一部の構文に使われる予約語が変数名の一部として認識されてしまう。
//例えば、forx=ytoz は、ytozが変数名として認識されてしまう。
//苦肉の策として、for[任意の文字列]toという構文を定義しておき、
//後から当関数にて[任意の文字列]を数値式としてパースする。
//渡された部分文字列を何としてパースするかは引数ruleとして渡される。
bool partial_parse(std::string const& part, ParserStatus& status, StringRule const& rule)
{
    //std::cout<<part<<std::endl;
    std::string::const_iterator first = part.begin();
    std::string::const_iterator last = part.end();

    bool r = qi::phrase_parse(first, last, rule, sw::blank);

    if (!r || first != last) {
        status.errorList_.push_back(ErrorInfo(status.textLineNumber_, status.basicLineNumber_, (boost::format("部分構文エラー(%1%)") % part).str()));
        return false;
    }

    return true;
}

bool program_parse(const std::string& program, ParserStatus& status)
{
    //グラフィック文字
    StringRule graph
            =   L("月")|L("火")|L("水")|L("木")|L("金")|L("土")|L("日")|L("年")|L("円")
            |	L("時")|L("分")|L("秒")|L("百")|L("千")|L("万")|L("π")
            |	L("┻")|L("┳")|L("┫")|L("┣")|L("╋")|L("┃")|L("━")|L("┏")|L("┓")|L("┗")|L("┛")
            |	L("×")|L("大")|L("中")|L("小")|L("▲")|L("▼")|L("★")|L("◆")|L("○")|L("●");

    //かな記号
    StringRule kana_kigou
            =	L("「")|L("」")|L("、")|L("。")|L("・")|L("゛")|L("゜")|L("ー");

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
            |	graph|kana_kigou|hiragana|katakana|han_kana
            ;

    //数値系--------------------------------------------------------------------------
    //数値関連
    StringRule num_expression, num_func, num_group;

    //予約語
    StringRule reserved
            =   lit("and")
            |   lit("xor")
            |   lit("or")
            |   lit("eqv")
            |   lit("imp")
            |   lit("mod");
    //数値型変数(5文字まで。識別されるのは2文字まで)
    //変数に予約語が含まれないようにする
    StringRule num_var
            =   sw::alpha >> qi::repeat(0, 4)[sw::alnum - reserved];

    //数値型変数(配列)
    //#PENDING DIM分との間の次元数チェック
    StringRule num_array_var
            =   num_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")");

    //数値リテラル
    StringRule sign
            =   L("+") | L("-");

    StringRule fractional_constant
            =  *sw::digit >> -L(".") >> +sw::digit
                             |  +sw::digit >> -L(".");

    StringRule exponent_part
            =   (L("e") | L("E")) >> -sign >> +sw::digit;

    StringRule num_literal
            =   -sign >> (fractional_constant >> -exponent_part
                          | +sw::digit >> exponent_part);

    //数値リテラル(16進)
    StringRule num_hex_literal
            =   L("&h") >> hex;

    //数値
    StringRule num_value
            =   num_func
            |   num_literal
            |   num_hex_literal
            |   num_array_var
            |   num_var
            |   num_group;

    //単項式
    StringRule unary_expression
            =   num_value
            |   (L("-") >> num_value)
            |   (L("not") >> num_value);

    //算術式
    StringRule arithmetic_operator
            =   L("+")
            |   L("-")
            |   L("*")
            |   L("/")
            |   L("^")
            |   L("\\") //整数除算
            |   L("mod");

    StringRule num_arithmetic_expression
            =   unary_expression >> *(arithmetic_operator > unary_expression);

    //関係式
    StringRule rel_operator
            =   L("<>")
            |   L("><")
            |   L("<=")
            |   L("=<")
            |   L(">=")
            |   L("=>")
            |   L("=")
            |   L("<")
            |   L(">");

    StringRule str_expression;
    StringRule rel_expression
            //文字列は"文字列式 比較演算子 文字列式"というフォーマットが成立している場合に限り関係式とみなす
            =   (str_expression >> (rel_operator > str_expression))
            |   (num_arithmetic_expression >> *(rel_operator > num_arithmetic_expression));

    //論理式
    StringRule logical_operator
            =   L("and")
            |   L("xor")
            |   L("or")
            |   L("eqv")
            |   L("imp");
    StringRule logical_expression
            =   (rel_expression >> *(logical_operator > rel_expression));

    //数値式
    num_expression
            =   logical_expression.alias();

    //数値グループ
    num_group
            =   L("(") >> num_expression >> L(")") ;


    //文字列系--------------------------------------------------------------------------
    //文字列変数($を抜いて5文字まで。識別されるのは2文字まで)
    StringRule str_var
            =   sw::alpha >> qi::repeat(0, 4)[sw::alnum] >> L("$");

    //文字列変数(配列)
    //#PENDING DIM分との間の次元数チェック
    StringRule str_array_var
            =   str_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")");

    //文字列リテラル(ダブルクオーテーションを含まない)
    //2つ目のダブルクオートの前に「-」が付いているのは、行末のダブルクオートは省略できるという仕様への対応
    StringRule str_literal
            =   *(printable - L("\""));

    //文字列リテラル(ダブルクオーテーションを含む)
    StringRule str_quoted
            =   L("\"") > str_literal > -L("\"");

    //文字列グループ
    StringRule str_group
            =   L("(") >> str_expression >> L(")");

    //文字列値
    StringRule str_func;
    StringRule str_value
            =   str_func
            |   str_quoted
            |   str_array_var
            |   str_var
            |   str_group;

    //文字列式
    str_expression
            =   str_value >> *(L("+") > str_value);

    //式
    StringRule expression
            =   str_expression | num_expression;

    //変数
    StringRule var
            =   str_array_var | str_var | num_array_var | num_var;

    //関数----------------------------------------------------------------------
    //数値型関数
    //ASC
    StringRule num_func_abs
            =   L("abs") >> L("(") > num_expression >> L(")");

    //ABS
    StringRule num_func_asc
            =   L("asc") >> L("(") > str_expression >> L(")");

    //COS
    StringRule num_func_cos
            =   L("cos") >> L("(") > num_expression >> L(")");

    //CSRLIN
    StringRule num_func_csrlin
            =   L("csrlin");

    //CVS
    StringRule num_func_cvs
            =   L("cvs") >> L("(") > str_expression >> L(")");

    //DEFFN文で定義された関数
    StringRule num_func_deffn
            =   L("fn") >> num_var >> L("(") > num_expression >> L(")");

    //DSKF
    StringRule num_func_dskf
            =   L("dskf") >> L("(") > num_expression
                          >> -(L(",") >> num_expression) >> L(")");

    //EOF
    StringRule num_func_eof
            =   L("eof") >> L("(") > num_expression >> L(")");

    //ERL,ERR
    //#PENDING ERL,ERRは現状数値変数の定義で代用できるため、定義しない。
    //変数の代入、参照の管理を実装する際に再検討する。

    //EXP
    StringRule num_func_exp
            =   L("exp") >> L("(") > num_expression >> L(")");

    //FRE
    StringRule num_func_fre
            =   L("fre") >> L("(") >> expression >> L(")");

    //INP
    StringRule num_func_inp
            =   L("inp") >> L("(") > num_expression >> L(")");

    //INT
    StringRule num_func_int
            =   L("int") >> L("(") > num_expression >> L(")");

    //LEN
    StringRule num_func_len
            =   L("len") >> L("(") > str_expression >> L(")");

    //LOC
    StringRule num_func_loc
            =   L("loc") >> L("(") > num_expression >> L(")");

    //LOF
    StringRule num_func_lof
            =   L("lof") >> L("(") > num_expression >> L(")");

    //LOG
    StringRule num_func_log
            =   L("log") >> L("(") > num_expression >> L(")");

    //LPOS
    StringRule num_func_lpos
            =   L("lpos") >> L("(") > num_expression >> L(")");

    //PAD
    StringRule num_func_pad
            =   L("pad") >> L("(") > num_expression >> L(")");

    //PEEK
    StringRule num_func_peek
            =   L("peek") >> num_expression;

    //POINT
    StringRule num_func_point
            =   L("point") >> -L("step")
                           >> L("(") > num_expression >> L(",") >> num_expression >> L(")");

    //POS
    StringRule num_func_pos
            =   L("pos") >> L("(") > num_expression >> L(")");

    //RND
    StringRule num_func_rnd
            =   L("rnd") >> L("(") > num_expression >> L(")");

    //ROLL
    StringRule num_func_roll
            =   L("roll") >> L("(") > num_expression >> L(")");

    //SCREEN
    StringRule num_func_screen
            =   L("screen") >> L("(") > num_expression >> L(",") >> num_expression >> L(")");

    //SGN
    StringRule num_func_sgn
            =   L("sgn") >> L("(") > num_expression >> L(")");

    //SIN
    StringRule num_func_sin
            =   L("sin") >> L("(") > num_expression >> L(")");

    //SQR
    StringRule num_func_sqr
            =   L("sqr") >> L("(") > num_expression >> L(")");

    //STICK
    StringRule num_func_stick
            =   L("stick") >> L("(") > num_expression >> L(")");

    //STRIG
    StringRule num_func_strig
            =   L("strig") >> L("(") > num_expression >> L(")");

    //TAN
    StringRule num_func_tan
            =   L("tan") >> L("(") > num_expression >> L(")");

    //TIME
    StringRule num_func_time
            =   L("time");

    //USR(数値を返す場合)
    //USR関数は引数に文字列を取るものと数字を取るものがあるため、
    //L("(") >> num_expressionのようにバックトラックを可能にしておく。
    StringRule num_func_usr
            =   L("usr") >> L("(") >> num_expression >> L(")");

    //VAL
    StringRule num_func_val
            =   L("val") >> L("(") > str_expression >> L(")");

    //数値関数
    num_func
            =   num_func_val
            |   num_func_usr
            |   num_func_time
            |   num_func_tan
            |   num_func_strig
            |   num_func_stick
            |   num_func_sqr
            |   num_func_sin
            |   num_func_sgn
            |   num_func_screen
            |   num_func_roll
            |   num_func_rnd
            |   num_func_pos
            |   num_func_point
            |   num_func_peek
            |   num_func_pad
            |   num_func_lpos
            |   num_func_log
            |   num_func_lof
            |   num_func_loc
            |   num_func_len
            |   num_func_int
            |   num_func_inp
            |   num_func_fre
            |   num_func_exp
            |   num_func_eof
            |   num_func_dskf
            |   num_func_deffn
            |   num_func_cvs
            |   num_func_csrlin
            |   num_func_cos
            |   num_func_asc
            |   num_func_abs;

    //文字列関数
    //CHR$
    StringRule str_func_chr$
            =   L("chr$") >> L("(") > num_expression >> L(")");

    //DSKI$
    StringRule str_func_dski$
            =   L("dski$") >> L("(") > num_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression > L(")");

    //GRP$
    StringRule str_func_grp$
            =   L("grp$") >> L("(") > num_expression >> L(")");

    //HEX$
    StringRule str_func_hex$
            =   L("hex$") >> L("(") > num_expression >> L(")");

    //INKEY$
    StringRule str_func_inkey$
            =   L("inkey$");

    //LEFT$
    StringRule str_func_left$
            =   L("left$") >> L("(") > str_expression >> L(",") >> num_expression >> L(")");

    //MID$
    StringRule str_func_mid$
            =   L("mid$") >> L("(") > str_expression
                          >> L(",") >> num_expression
                          >> -(L(",") >> num_expression)
                          >> L(")");

    //OCT$
    StringRule str_func_oct$
            =   L("oct$") >> L("(") > num_expression >> L(")");

    //RIGHT$
    StringRule str_func_right$
            =   L("right$") >> L("(") > str_expression >> L(",") >> num_expression >> L(")");
    //SPACE$
    StringRule str_func_space$
            =   L("space$") >> L("(") > num_expression >> L(")");

    //STR$
    StringRule str_func_str$
            =   L("str$") >> L("(") > num_expression >> L(")");

    //TIME$
    StringRule str_func_time$
            =   L("time$") >> L("(") > num_expression >> L(")");

    //USR(文字列を返す場合)
    //USR関数は引数に文字列を取るものと数字を取るものがあるため、
    //L("(") >> str_expressionのようにバックトラックを可能にしておく。
    StringRule str_func_usr
            =   L("usr") >> L("(") >> str_expression >> L(")");

    //文字列関数
    str_func
            =   str_func_usr
            |   str_func_time$
            |   str_func_str$
            |   str_func_space$
            |   str_func_right$
            |   str_func_oct$
            |   str_func_mid$
            |   str_func_left$
            |   str_func_inkey$
            |   str_func_hex$
            |   str_func_grp$
            |   str_func_dski$
            |   str_func_chr$
            ;

    //行番号
    UintRule linenumber = uint_;

    //ステートメント、コマンド------------------------------------------------
    StringRule statement;
    //代入文
    StringRule num_assign
            =   (num_array_var | num_var ) >> L("=")
                                              > qi::as_string[*(char_ - lit(":"))]
                                              [phx::bind(&partial_parse, _1, ref(status), num_expression)];
    StringRule str_assign
            =   (str_array_var | str_var ) >> L("=") > str_expression;

    //グラフィック用２次元座標
    StringRule coord_2d
            =   L("(") >> num_expression >> L(",") >> num_expression >> L(")");

    //AUTO文
    StringRule st_auto
            =   L("auto") >> -linenumber >> -(L(",") > linenumber);

    //BGM文
    StringRule st_bgm
            =   L("bgm") >> num_expression;

    //BLOAD文
    StringRule st_bload
            =   L("bload") >> str_expression
                           >> -(L(",") >> -num_expression)
                           >> -(L(",") > -L("r"));
    //BSAVE文
    StringRule st_bsave
            =   L("bsave") >> str_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression;

    //CIRCLE文
    StringRule st_circle
            =   L("circle") >> -L("step") >> L("(")
                            >> num_expression >> L(",")     //x
                            >> num_expression >> L(")")     //y
                            >> L(",") >> num_expression     //r
                            >> -(L(",") >> -num_expression)   //c
                            >> -(L(",") >> -num_expression)   //s
                            >> -(L(",") >> -num_expression)   //e
                            >> -(L(",") > -num_expression);   //a

    //CLEAR文
    StringRule st_clear
            =   L("clear") >> -num_expression
                           >> -(L(",") > num_expression);

    //CLOAD*文
    //#PENDING 配列名の存在確認
    StringRule st_cload_ast
            =   L("cload") >> L("*") >> num_var;

    //CLOAD文(CLOAD?文も兼ねる)
    StringRule st_cload
            =   L("cload") >> -L("?") >> -str_expression;

    //CLOSE文
    StringRule st_close
            =   L("close") >> -(num_expression
                                >> *(L(",") >> num_expression));
    //CLS文
    StringRule st_cls
            =   L("cls");

    //COLOR文
    StringRule st_color
            =   L("color") >> -num_expression               //f
                           >> -(L(",") >> -num_expression)  //b
                           >> -(L(",") > -num_expression);  //c

    //CONSOLE文
    StringRule st_console
            =   L("console") >> -num_expression               //m
                             >> -(L(",") >> -num_expression)    //n
                             >> -(L(",") >> -num_expression)    //f
                             >> -(L(",") >> -num_expression)    //k
                             >> -(L(",") > -num_expression);    //p

    //CONT文
    //プログラム中に存在し得ないはずだが、一応定義しておく。
    StringRule st_cont
            =   L("cont");

    //CSAVE文
    StringRule st_csave
            =   L("csave") >> str_expression;

    //CSAVE*文
    StringRule st_csave_ast
            =   L("csave") >> L("*") >> num_var;

    //DATA文
    StringRule data_element
            =   +(printable - L("\"") - L(",") - L(":"))
            |   (L("\"") >> str_literal >> -L("\""));
    StringRule st_data
            =   L("data") >> -data_element
                          >> *(L(",") > -data_element);

    //DEFFN文
    //#PENDING 登録された関数名の管理
    StringRule st_deffn
            =   L("def") >> L("fn") >> num_var
                         >> L("(") >> num_var >> L(")") >> L("=")
                         >> num_expression;

    //DEFUSR文
    StringRule st_defusr
            =   L("def") >> L("usr") >> L("=") >> num_expression;

    //DELETE文
    StringRule st_delete
            =   L("delete") >> (linenumber || (L("-") >> -linenumber));

    //DIM文
    //#PENDING配列変数名の管理
    StringRule array_var
            =   str_array_var | num_array_var;
    StringRule st_dim
            =   L("dim") >> array_var
                         >> *(L(",") > array_var);

    //DSKO$文
    StringRule st_dsko$
            =   L("dsko$") >> L("(") >> num_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression > L(")");

    //END文
    StringRule st_end
            =   L("end");

    //ERASE文
    StringRule st_erase
            =   L("erase") >> array_var
                           >> *(L(",") > array_var);

    //ERROR文
    StringRule st_error
            =   L("error") >> num_expression;

    //EXEC文
    StringRule st_exec
            =   L("exec") >> num_expression;

    //FIELD文
    StringRule st_field
            =   L("field") >> -L("#")
                           >> num_expression
                           >> +(L(",")
                                >> qi::as_string[*(char_ - lit("as"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
                                >> L("as") >> str_var);

    //FILES文
    StringRule st_files
            =   L("files") >> -num_expression;

    //FOR〜NEXT文
    //Spiritのルールは通常先頭から最長一致でマッチしてしまうため、
    //一部の構文に使われる予約語が変数名の一部として認識されてしまう。
    //例えば、forx=ytoz は、ytozが変数名として認識されてしまう。
    //苦肉の策として、for[任意の文字列]toという構文を定義しておき、
    //後からpartial_parse関数にて[任意の文字列]を数値式としてパースする。
    StringRule st_for
            =   L("for") > num_var >> L("=")
                                      > qi::as_string[*(char_ - lit("to"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
                                   >> L("to")
                                      > qi::as_string[*(char_ - lit("step") - lit(":"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
                                   >> -(L("step") >> num_expression);
    StringRule st_next
            =   L("next") >> -(num_var >> *(L(",") > num_var));

    //GET文
    StringRule st_get
            =   L("get") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //GET@文
    //#PENDING 配列変数名の管理
    StringRule st_get_at
            =   L("get") >> -L("@") >> -L("step")
                         >> coord_2d
                         >> L("-") >> -L("step")
                         >> coord_2d
                         >> L(",") >> (str_expression | num_var);

    //GOSUB〜RETURN文
    StringRule st_gosub
            =   L("gosub") >> linenumber;
    StringRule st_return
            =   L("return");

    //GOTO文
    //PC-6000Techknowによると、「g o t o」でも通るため、トークンを分ける
    StringRule st_goto
            =   L("g") >> L("o")  >> L("t") >> L("o") > linenumber;

    //IF文
    StringRule st_then
            =   (L("then") >> statement >> *(L(":") >> statement))
            |   (L("then") >> linenumber)
            |   st_goto;
    StringRule st_if
            =   L("if") >> qi::as_string[*(char_ - lit("then") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), logical_expression)]
                        >> qi::as_string[*(char_ - lit("else"))]
                           [phx::bind(&partial_parse, _1, ref(status), st_then)]
                        >> -((L("else") >> statement)
                            | (L("else") >> linenumber));

    //INPUT文
    StringRule st_input
            =   L("input") >> -(str_expression >> L(";"))
                           >> var
                           >> *(L(",") >> var);

    //INPUT#文
    StringRule st_input_sharp
            =   L("input") >> L("#") >> num_expression
                           >> +(L(",") >> var);
    //KANJI文
    StringRule st_kanji
            =   L("kanji") >> -L("step")
                           >> coord_2d
                           >> L(",") >> num_expression //色
                           >> L(",") >> expression
                           >> *(-L(",") >> expression);
    //KEY文
    StringRule st_key
            =   L("key") >> num_expression >> L(",") >> str_expression;

    //KILL文
    StringRule st_kill
            =   L("kill") >> str_expression;

    //LCOPY文
    StringRule st_lcopy
            =   L("lcopy") >> -num_expression;

    //LET文
    StringRule st_let
            =   L("let") >> (str_assign | num_assign);

    //LFILES文
    StringRule st_lfiles
            =   L("lfiles") >> num_expression;

    //LINE文
    StringRule st_line
            =   L("line") >> -(-L("step")
                          >> coord_2d)
                          >> L("-") >> -L("step")
                          >> coord_2d
                          >> -(L(",") >> -num_expression) //色
                          >> -(L(",") >> (L("bf") | L("b")));

    //LIST文
    StringRule st_list
            =   L("list") >> (linenumber || (L("-") >> -linenumber));

    //LIST L文
    StringRule st_list_l
            =   L("list") >> L("l") >> -(L(",") >> linenumber);

    //LIST V文
    StringRule st_list_v
            =   L("list") >> L("v") >> -(L(",") >> var);

    //LLIST文
    StringRule st_llist
            =   L("llist") >> (linenumber || (L("-") >> -linenumber));

    //LLIST L文
    StringRule st_llist_l
            =   L("llist") >> L("l") >> -(L(",") >> linenumber);

    //LLIST V文
    StringRule st_llist_v
            =   L("llist") >> L("v") >> -(L(",") >> var);

    //LOAD文
    StringRule st_load
            =   L("load") >> str_expression >> -(L(",") >> L("r"));

    //LOCATE文(LOCATE@にも対応)
    StringRule st_locate
            =   L("locate") >> -L("@") >> -num_expression
                          >> -(L(",") >> -num_expression)
                          >> -(L(",") >> num_expression);

    //LPRINT文
    //SPC,TAB関数はPRINT,LPRINT中でのみ使える。
    StringRule str_func_spc
            =   L("spc") >> L("(") >> num_expression >> L(")");
    StringRule str_func_tab
            =   L("spc") >> L("(") >> num_expression >> L(")");
    //PRINT対象文字列
    StringRule str_print
            =   expression | str_func_spc | str_func_tab;
    StringRule st_lprint
            =   (L("lprint")|L("?")) >> *((L(";") | L(",")) || str_print);

    //LSET文
    StringRule st_lset
            =   L("lset") >> str_assign;

    //MENU文
    StringRule st_menu
            =   L("menu");

    //MERGE文
    StringRule st_merge
            =   L("merge") >> str_expression;

    //NAME文
    StringRule st_name
            =   L("name") >> str_expression >> L("as") >> str_expression;

    //NEW文(こんなのプログラム中に有るのか？)
    StringRule st_new
            =   L("new");

    //ON ERROR GOTO文
    StringRule st_on_error_goto
            =   L("on") >> L("error") > st_goto;

    //ON GOSUB文
    StringRule st_on_gosub
            =   L("on") >> !L("error")
                        >> qi::as_string[*(char_ - lit("gosub") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("gosub")
                        > -linenumber
                        >> *(L(",") > -linenumber);

    //ON GOTO文
    StringRule st_on_goto
            =   L("on") >> !L("error")
                        >> qi::as_string[*(char_ - lit("goto") - lit("gosub"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("goto")
                        > -linenumber
                        >> *(L(",") > -linenumber);

    //OPEN文
    StringRule st_open
            =   L("open") >> str_expression
                          >> -(L("for") >> (L("input") | L("output") | L("append")))
                          >> L("as") >> -L("#") >> num_expression;

    //OUT文
    StringRule st_out
            =   L("out") >> num_expression >> L(",") >> num_expression;

    //PAINT文
    StringRule st_paint
            =   L("paint") >> -L("step")
                           >> coord_2d
                           >> -(L(",") >> -num_expression) //領域色
                           >> -(L(",") >> num_expression); //境界色

    //PALET文
    StringRule st_palet
            =   L("palet") >> -(num_expression >> L(",") >> num_expression);

    //PLAY文
    //#PENDING MML構文チェック
    StringRule st_play
            =   L("play") >> -str_expression
                          >> qi::repeat(0, 4)[(L(",") >> -str_expression)]
                          >> -(L(",") >> str_expression);
    //POKE文
    StringRule st_poke
            =   L("poke") >> num_expression >> L(",") >> num_expression;

    //PRESET文
    StringRule st_preset
            =   L("preset") >> -L("step")
                            >> coord_2d;

    //PRINT文(PRINT@にも対応)
    StringRule st_print
            =   (L("print")|L("?")) >> -L("@") >> *((L(";") | L(",")) || str_print);

    //PRINT#文
    StringRule st_print_sharp
            =   L("print") >> L("#") >> num_expression
                           >> L(",") >> expression
                           >> *((L(";") | L(",")) > expression);

    //PSET文
    StringRule st_pset
            =   L("pset") >> -L("step")
                          >> coord_2d
                          >> -(L(",") >> num_expression); //色
    //PUT文
    StringRule st_put
            =   L("put") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //PUT@文
    StringRule st_put_at
            =   L("put") >> -L("@") >> -L("step")
                         >> coord_2d
                         >> L(",") >> (num_var | str_expression)
                         >> -(L(",") >> (L("xor") | L("and") | L("or") | L("pset") | L("preset"))); //色

    //READ文
    StringRule st_read
            =   L("read") >> (str_array_var | str_var | num_array_var | num_var)
                         >> *(L(",") >> (str_array_var | str_var | num_array_var | num_var));

    //REM文
    StringRule st_rem
            =   (L("rem") | L("'")) >> *printable;

    //RENUM文
    StringRule st_renum
            =   L("renum") >> -linenumber >> -(L(",") >> linenumber);

    //RESTORE文
    StringRule st_restore
            =   L("restore") >> -linenumber;

    //RESUME文
    StringRule st_resume
            =   L("resume") >> -(L("0") | L("next") | linenumber);

    //ROLL文
    StringRule st_roll
            =   L("roll") >> -num_expression
                          >> -(L(",") >> -num_expression)
                          >> -(L(",") >> L("y"));

    //RSET文
    StringRule st_rset
            =   L("rset") >> str_assign;

    //RUN文
    StringRule st_run
            =   L("run") >> -(linenumber
                              |(str_expression >> -(L(",") >> L("r"))));

    //SAVE文
    StringRule st_save
            =   L("save") >> str_expression >> -(L(",") >> L("a"));

    //SCREEN文
    StringRule st_screen
            =   L("screen") >> -num_expression               //m
                            >> -(L(",") >> -num_expression)    //a
                            >> -(L(",") > num_expression);     //V

    //SOUND文
    StringRule st_sound
            =   L("sound") >> num_expression >> L(",") >> num_expression;

    //STOP文
    StringRule st_stop
            =   L("stop");

    //TALK文
    //#PENDING TALK文構文チェック
    StringRule st_talk
            =   L("talk") >> str_expression;

    //TROFF文
    StringRule st_troff
            =   L("troff");

    //TRON文
    StringRule st_tron
            =   L("tron");

    //WAIT文
    StringRule st_wait
            =   L("wait") >> num_expression
                          >> L(",") >> -num_expression
                          >> -(L(",") > num_expression);

    //WIDTH文
    StringRule st_width
            =   L("width") >> -num_expression
                           >> -(L(",") > num_expression);

    //文
    statement
            =   st_width
            |   st_wait
            |   st_tron
            |   st_troff
            |   st_talk
            |   st_stop
            |   st_sound
            |   st_screen
            |   st_save
            |   st_run
            |   st_rset
            |   st_roll
            |   st_resume
            |   st_restore
            |   st_renum
            |   st_rem
            |   st_read
            |   st_put_at
            |   st_put
            |   st_pset
            |   st_print_sharp
            |   st_print
            |   st_preset
            |   st_poke
            |   st_play
            |   st_palet
            |   st_paint
            |   st_out
            |   st_open
            |   st_on_goto
            |   st_on_gosub
            |   st_on_error_goto
            |   st_new
            |   st_name
            |   st_merge
            |   st_menu
            |   st_lset
            |   st_lprint
            |   st_locate
            |   st_load
            |   st_llist_v
            |   st_llist_l
            |   st_llist
            |   st_list_v
            |   st_list_l
            |   st_list
            |   st_line
            |   st_lfiles
            |   st_let
            |   st_lcopy
            |   st_kill
            |   st_key
            |   st_kanji
            |   st_input_sharp
            |   st_input
            |   st_if
            |   st_goto
            |   st_gosub | st_return
            |   st_get_at
            |   st_get
            |   st_for  |   st_next
            |   st_files
            |   st_field
            |   st_exec
            |   st_error
            |   st_erase
            |   st_end
            |   st_dsko$
            |   st_dim
            |   st_delete
            |   st_defusr
            |   st_deffn
            |   st_data
            |   st_csave_ast
            |   st_csave
            |   st_cont
            |   st_console
            |   st_color
            |   st_cls
            |   st_close
            |   st_cload_ast
            |   st_cload
            |   st_clear
            |   st_circle
            |   st_bsave
            |   st_bload
            |   st_bgm
            |   st_auto
            |   num_assign
            |   str_assign;

    //行
    StringRule line
            =   linenumber[ref(status.basicLineNumber_) = _1]
            >   +(L(":") || statement);

    std::string::const_iterator first = program.begin();
    std::string::const_iterator last = program.end();

    bool r = qi::phrase_parse(first, last, line, sw::blank);

    if (!r || first != last) {
        return false;
    }

    return true;
}

Checker::Checker()
{
    //全角半角テーブルを構築
    makeZenHanMap();
}

struct printer
{
    typedef boost::spirit::utf8_string string;

    void element(string const& tag, string const& value, int depth) const
    {
        for (int i = 0; i < (depth*4); ++i) // indent to depth
            std::cout << ' ';

        std::cout << "tag: " << tag;
        if (value != "")
            std::cout << ", value: " << value;
        std::cout << std::endl;
    }
};

void print_info(boost::spirit::info const& what)
{
    using boost::spirit::basic_info_walker;

    printer pr;
    basic_info_walker<printer> walker(pr, what.tag, 0);
    boost::apply_visitor(walker, what.value);
}

bool Checker::parse(const std::string& programList, ParserStatus& stat, bool trace)
{
    //作業用プログラムリスト
    std::string workProgramList = programList;
    //プリプロセス。
    //全角を半角に
    convZenHan(workProgramList);
    //大文字を小文字に
    transform(workProgramList.begin (), workProgramList.end (), workProgramList.begin (), tolower);

    stat = ParserStatus();
    //プログラムを行ごとに分割
    std::vector<std::string> list;
    boost::algorithm::split(list, workProgramList, boost::is_any_of(L"\n"));

    bool result = true;
    for(size_t i = 0; i < list.size(); i++){
        stat.inclementLine();
        const std::string line = list[i];
        if (line.empty()) continue;

        // 1行の構文解析結果を判定
        bool r = true;
        try{
            r = program_parse(line, stat);
        }
        catch (qi::expectation_failure<std::string::const_iterator> const& x)
        {
            if(trace){
                std::cout << "expected: "; print_info(x.what_);
                std::cout << "got: " << std::string(x.first, x.last) << std::endl;
                std::cout << "textLine: " << stat.textLineNumber_ << std::endl;
                std::cout << "basicline: " << stat.basicLineNumber_ << std::endl;
            }
            r = false;
        }
        if(!r){
            stat.errorList_.push_back(ErrorInfo(stat.textLineNumber_, stat.basicLineNumber_, "構文エラー"));
        }

        result &= r;
    }
    return result;
}
