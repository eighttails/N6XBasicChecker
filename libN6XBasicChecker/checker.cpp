#include <iostream>
#include <string>
#include <algorithm>

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
#define L(a)    lit(a)

template <typename Iterator>
bool program_parse(Iterator first, Iterator last, ParserStatus& status)
{
    using sw::char_;
    using qi::_1;
    using qi::int_;
    using qi::uint_;
    using qi::double_;
    using qi::hex;
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

    //数値系--------------------------------------------------------------------------
    //数値関連
    StringRule num_expression, num_func, num_group;

    //数値型変数(5文字まで。識別されるのは2文字まで)
    //予約語を含まないようにする。
    StringRule num_var
            =   sw::alpha >> qi::repeat(0, 4)
                             [sw::alnum
                             - L("as")
                             - L("to")
                             - L("goto")
                             - L("step")
                             - L("then")
                             - L("else")];

    //数値型変数(配列)
    //#PENDING DIM分との間の次元数チェック
    StringRule num_array_var
            =   num_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")");

    //数値リテラル
    StringRule sign
            =   L("+") | L("-");

    StringRule fractional_constant
            =  *sw::digit >> '.' >> +sw::digit
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
            |   L("or");
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
    StringRule str_literal
            =   *(printable - L("\""));


    //文字列グループ
    StringRule str_group
            =   L("(") >> str_expression >> L(")");

    //文字列値
    StringRule str_func;
    StringRule str_value
            =   str_func
            //2つ目のダブルクオートの前に「-」が付いているのは、行末のダブルクオートは省略できるという仕様への対応
            |   (L("\"") > str_literal > -L("\""))
            |   str_array_var
            |   str_var
            |   str_group;

    //文字列式
    str_expression
            =   str_value >> *(L("+") > str_value);

    //式
    StringRule expression
            =   str_expression | num_expression;

    //関数----------------------------------------------------------------------
    //数値型関数
    //ASC
    StringRule num_func_abs
            =   L("abs") >> L("(") > num_expression > L(")");

    //ABS
    StringRule num_func_asc
            =   L("asc") >> L("(") > str_expression > L(")");

    //COS
    StringRule num_func_cos
            =   L("cos") >> L("(") > num_expression > L(")");

    //CSRLIN
    StringRule num_func_csrlin
            =   L("csrlin");

    //CVS
    StringRule num_func_cvs
            =   L("cvs") >> L("(") >> str_expression >> L(")");

    //DEFFN文で定義された関数
    StringRule num_func_deffn
            =   L("fn") >> num_var >> L("(") >> num_expression >> L(")");

    //DSKF
    StringRule num_func_dskf
            =   L("dskf") >> L("(") >> num_expression
                          >> -(L(",") >> num_expression) >> L(")");

    //EOF
    StringRule num_func_eof
            =   L("eof") >> L("(") >> num_expression >> L(")");

    //ERL,ERR
    //#PENDING ERL,ERRは現状数値変数の定義で代用できるため、定義しない。
    //変数の代入、参照の管理を実装する際に再検討する。

    //EXP
    StringRule num_func_exp
            =   L("exp") >> L("(") >> num_expression >> L(")");

    //INP
    StringRule num_func_inp
            =   L("inp") >> L("(") >> num_expression >> L(")");

    //INT
    StringRule num_func_int
            =   L("int") >> L("(") >> num_expression >> L(")");

    //LEN
    StringRule num_func_len
            =   L("len") >> L("(") >> str_expression >> L(")");

    //LOC
    StringRule num_func_loc
            =   L("loc") >> L("(") >> num_expression >> L(")");

    //LOF
    StringRule num_func_lof
            =   L("lof") >> L("(") >> num_expression >> L(")");

    //LOG
    StringRule num_func_log
            =   L("log") >> L("(") >> num_expression >> L(")");

    //LPOS
    StringRule num_func_lpos
            =   L("lpos") >> L("(") >> num_expression >> L(")");

    num_func
            =   num_func_lpos
            |   num_func_log
            |   num_func_lof
            |   num_func_loc
            |   num_func_len
            |   num_func_int
            |   num_func_inp
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
            =   L("chr$") >> L("(") >> num_expression >> L(")");

    //DSKI$
    StringRule str_func_dski$
            =   L("dski$") >> L("(") >> num_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression > L(")");

    //GRP$
    StringRule str_func_grp$
            =   L("grp$") >> L("(") >> num_expression >> L(")");

    //HEX$
    StringRule str_func_hex$
            =   L("hex$") >> L("(") >> num_expression >> L(")");

    //INKEY$
    StringRule str_func_inkey$
            =   L("inkey$");

    //LEFT$
    StringRule str_func_left$
            =   L("left$") >> L("(") >> str_expression >> L(",") >> num_expression >> L(")");

    str_func
            =   str_func_left$
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
            =   (num_array_var | num_var ) >> L("=") > num_expression;
    StringRule str_assign
            =   (str_array_var | str_var ) >> L("=") > str_expression;

    //AUTO文
    StringRule st_auto
            =   L("auto") >> -linenumber >> -(L(",") > linenumber);

    //BGM文
    StringRule st_bgm
            =   L("bgm") >> num_expression;

    //BLOAD文
    StringRule st_bload
            =   L("bload") >> str_expression
                           >> -(L"," >> -num_expression)
                           >> -(L"," > -L("r"));
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
                            >> -(L"," >> -num_expression)   //c
                            >> -(L"," >> -num_expression)   //s
                            >> -(L"," >> -num_expression)   //e
                            >> -(L"," > -num_expression);   //a

    //CLEAR文
    StringRule st_clear
            =   L("clear") >> num_expression
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
                           >> -(L"," >> -num_expression)    //b
                           >> -(L"," > -num_expression);    //c

    //CONSOLE文
    StringRule st_console
            =   L("console") >> -num_expression               //m
                             >> -(L"," >> -num_expression)    //n
                             >> -(L"," >> -num_expression)    //f
                             >> -(L"," >> -num_expression)    //k
                             >> -(L"," > -num_expression);    //p

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
            =   +(printable - L("\"") - L(","))
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
            =   L("field") >> -L("#") >> num_expression
                           >> +(L(",") >> num_expression >> L("as") >> str_var);

    //FILES文
    StringRule st_files
            =   L("files") >> -num_expression;

    //FOR〜NEXT文
    StringRule st_for
            =   L("for") >> num_var >> L("=")
                         >> (num_expression)
                         >> L("to")
                         >> (num_expression - L("step"))
                         >> -(L("step") >> num_expression);
    StringRule st_next
            =   L("next") >> -num_var;

    //FRE文
    StringRule st_fre
            =   L("fre") >> L("(") >> expression >> L(")");

    //GET文
    StringRule st_get
            =   L("get") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //GET@文
    //#PENDING 配列変数名の管理
    StringRule st_get_at
            =   L("get") >> -L("@") >> -L("step")
                         >> L("(") >> num_expression >> L(",") >> num_expression >> L(")")
                         >> L("-") >> -L("step")
                         >> L("(") >> num_expression >> L(",") >> num_expression >> L(")")
                         >> L(",") >> (str_expression | num_var);

    //GOSUB〜RETURN文
    StringRule st_gosub
            =   L("gosub") >> linenumber;
    StringRule st_return
            =   L("return");

    //GOTO文
    //goとtoの間には空白を許容するため、トークンを分ける
    StringRule st_goto
            =   L("go") >> L("to") > linenumber;

    //IF文
    StringRule st_if
            =   L("if") >> rel_expression
                        >> ((L("then") >> statement)
                            | (L("then") >> linenumber)
                            | st_goto)
                        >> -((L("else") >> statement)
                            | (L("else") >> linenumber));

    //INPUT文
    StringRule st_input
            =   L("input") >> -(str_expression >> L(";"))
                           >> (str_array_var | str_var)
                           >> *(L(",") >> (str_array_var | str_var));

    //INPUT#文
    StringRule st_input_sharp
            =   L("input") >> L("#") >> num_expression
                           >> +(L(",") >> (num_array_var | num_var));
    //KANJI文
    StringRule st_kanji
            =   L("kanji") >> -L("step")
                           >> L("(") >> num_expression >> L(",") >> num_expression >> L(")")
                           >> L(",") >> num_expression //色
                           >> +(L(",") >> (expression));
    //KEY文
    StringRule st_key
            =   L("key") >> num_expression >> L(",") >> str_expression;

    //KILL文
    StringRule st_kill
            =   L("kill") >> str_expression;

    //LCOPY文
    StringRule st_lcopy
            =   L("lcopy") >> num_expression;

    //LET文
    StringRule st_let
            =   L("let") >> (str_assign | num_assign);

    //LFILES文
    StringRule st_lfiles
            =   L("lfiles") >> num_expression;

    //LINE文
    StringRule st_line
            =   L("line") >> -L("step")
                           >> L("(") >> num_expression >> L(",") >> num_expression >> L(")")
                           >> L("-") >> -L("step")
                           >> L("(") >> num_expression >> L(",") >> num_expression >> L(")")
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
            =   L("list") >> L("v") >> -(L(",")
                                         >> (str_array_var | str_var | num_array_var | num_var));
    //LLIST文
    StringRule st_llist
            =   L("llist") >> (linenumber || (L("-") >> -linenumber));

    //LLIST L文
    StringRule st_llist_l
            =   L("llist") >> L("l") >> -(L(",") >> linenumber);

    //LLIST V文
    StringRule st_llist_v
            =   L("llist") >> L("v") >> -(L(",")
                                         >> (str_array_var | str_var | num_array_var | num_var));

    //LOAD文
    StringRule st_load
            =   L("load") >> str_expression >> -(L(",") >> L("r"));

    //LOCATE文
    StringRule st_locate
            =   L("locate") >> -num_expression
                          >> -(L(",") >> -num_expression)
                          >> -(L(",") >> num_expression);
    //PRINT文
    StringRule st_lprint
            =   (L("lprint")|L("?")) >> expression >> *((L(";") | L(",")) > expression);

    //LSET文
    StringRule st_lset
            =   L("lset") >> str_assign;

    //PRINT文
    StringRule st_print
            =   (L("print")|L("?")) >> expression >> *((L(";") | L(",")) > expression);

    //RSET文
    StringRule st_rset
            =   L("rset") >> str_assign;

    //文
    statement
            =   st_rset
            |   st_print
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
            |   st_fre
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
            >   statement
            >>  *(+L(":") > statement);

    bool r = qi::phrase_parse(first, last, line, sw::blank);

    if (!r || first != last) {
        return false;
    }

    return true;
}

Checker::Checker()
{
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
    //#PENDING 全角を半角に
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
        std::string::const_iterator iter = line.begin(), end = line.end();

        bool r = true;
        try{
            r = program_parse(iter, end, stat);
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
