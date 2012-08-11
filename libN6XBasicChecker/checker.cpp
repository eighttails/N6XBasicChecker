#include <iostream>
#include <string>
#include <algorithm>

#include "checker.h"
#include "parserstatus.h"
#include "spiritwrap.h"

//部分パーサー
//Spiritのルールは通常先頭から最長一致でマッチしてしまうため、
//一部の構文に使われる予約語が変数名の一部として認識されてしまう。
//例えば、forx=ytoz は、ytozが変数名として認識されてしまう。
//苦肉の策として、for[任意の文字列]toという構文を定義しておき、
//後から当関数にて[任意の文字列]を数値式としてパースする。
//渡された部分文字列を何としてパースするかは引数ruleとして渡される。
bool partial_parse(std::wstring const& part, ParserStatus& status, BasicRule const& rule)
{
    std::wstring::const_iterator first = part.begin();
    std::wstring::const_iterator last = part.end();

    bool r = qi::phrase_parse(first, last, rule, sw::blank);

    if (!r || first != last) {
        //出力の際は大文字に変換partial_parse
        std::wstring workPart = part;
        transform(workPart.begin (), workPart.end (), workPart.begin (), toupper);

        status.errorList_.push_back(ErrorInfo(E_PART_SYNTAX, status.line_.textLineNumber_, status.line_.basicLineNumber_, (boost::wformat(L"部分シンタックスエラー[%1%]") % workPart).str()));
        return false;
    }

    return true;
}

bool program_parse(const std::wstring& program, ParserStatus& status)
{
    //グラフィック文字
    BasicRule graph
            =   L("月")|L("火")|L("水")|L("木")|L("金")|L("土")|L("日")|L("年")|L("円")
            |	L("時")|L("分")|L("秒")|L("百")|L("千")|L("万")|L("π")
            |	L("┻")|L("┳")|L("┫")|L("┣")|L("╋")|L("┃")|L("━")|L("┏")|L("┓")|L("┗")|L("┛")
            |	L("×")|L("大")|L("中")|L("小")|L("▲")|L("▼")|L("★")|L("◆")|L("○")|L("●");

    //かな記号
    BasicRule kana_kigou
            =	L("「")|L("」")|L("、")|L("。")|L("・")|L("゛")|L("゜")|L("ー");

    //ひらがな
    BasicRule hiragana
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
    BasicRule katakana
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
    BasicRule han_kana
            =	L("｡")|L("｢")|L("｣")|L("､")|L("･")|L("ｰ")
            |   L("ｦ")|L("ｧ")|L("ｨ")|L("ｩ")|L("ｪ")|L("ｫ")|L("ｬ")|L("ｭ")|L("ｮ")|L("ｯ")
            |	L("ｱ")|L("ｲ")|L("ｳ")|L("ｴ")|L("ｵ")|L("ｶ")|L("ｷ")|L("ｸ")|L("ｹ")|L("ｺ")
            |	L("ｻ")|L("ｼ")|L("ｽ")|L("ｾ")|L("ｿ")|L("ﾀ")|L("ﾁ")|L("ﾂ")|L("ﾃ")|L("ﾄ")
            |	L("ﾅ")|L("ﾆ")|L("ﾇ")|L("ﾈ")|L("ﾉ")|L("ﾊ")|L("ﾋ")|L("ﾌ")|L("ﾍ")|L("ﾎ")
            |	L("ﾏ")|L("ﾐ")|L("ﾑ")|L("ﾒ")|L("ﾓ")|L("ﾔ")|L("ﾕ")|L("ﾖ")
            |	L("ﾗ")|L("ﾘ")|L("ﾙ")|L("ﾚ")|L("ﾛ")|L("ﾜ")|L("ﾝ")
            |   L("ﾞ")|L("ﾟ");

    //使用可能な文字
    BasicRule printable
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
    BasicRule num_expression, num_func, num_group;

    //予約語
    BasicRule reserved
            =   lit("and")
            |   lit("xor")
            |   lit("or")
            |   lit("eqv")
            |   lit("imp")
            |   lit("mod");
    //数値型変数(5文字まで。識別されるのは2文字まで)
    //変数に予約語が含まれないようにする
    StringRule num_var
            =   qi::raw[sw::alpha >> qi::repeat(0, 4)[sw::alnum - reserved]];

    //数値型変数(配列)
    StringRule num_array_var
            =   qi::raw[num_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")")];

    //数値リテラル
    BasicRule sign
            =   L("+") | L("-");

    BasicRule fractional_constant
            =  *sw::digit >> -L(".") >> +sw::digit
                             |  +sw::digit >> -L(".");

    BasicRule exponent_part
            =   (L("e") | L("E")) >> -sign >> +sw::digit;

    BasicRule num_literal
            =   -sign >> (fractional_constant >> -exponent_part
                          | +sw::digit >> exponent_part);

    //数値リテラル(16進)
    BasicRule num_hex_literal
            =   L("&h") >> hex;

    //数値
    BasicRule num_value
            =   num_func
            |   num_literal
            |   num_hex_literal
            |   num_array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"num_value")]
            |   num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"num_value")]
            |   num_group;

    //単項式
    BasicRule unary_expression
            =   (L("-") >> num_value)
            |   (L("not") >> num_value)
            |   num_value;

    //算術式
    BasicRule arithmetic_operator
            =   L("+")
            |   L("-")
            |   L("*")
            |   L("/")
            |   L("^")
            |   L("\\") //整数除算
            |   L("mod");

    BasicRule num_arithmetic_expression
            =   unary_expression >> *(arithmetic_operator > unary_expression);

    //関係式
    BasicRule rel_operator
            =   L("<>")
            |   L("><")
            |   L("<=")
            |   L("=<")
            |   L(">=")
            |   L("=>")
            |   L("=")
            |   L("<")
            |   L(">");

    BasicRule str_expression;
    BasicRule rel_expression
            //文字列は"文字列式 比較演算子 文字列式"というフォーマットが成立している場合に限り関係式とみなす
            =   (str_expression >> (rel_operator > str_expression))
            |   (num_arithmetic_expression >> *(rel_operator > num_arithmetic_expression));

    //論理式
    BasicRule logical_operator
            =   L("and")
            |   L("xor")
            |   L("or")
            |   L("eqv")
            |   L("imp");
    BasicRule logical_expression
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
            =   qi::raw[sw::alpha >> qi::repeat(0, 4)[sw::alnum] >> L("$")];

    //文字列変数(配列)
    StringRule str_array_var
            =   qi::raw[str_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")")];

    //文字列リテラル(ダブルクオーテーションを含まない)
    //2つ目のダブルクオートの前に「-」が付いているのは、行末のダブルクオートは省略できるという仕様への対応
    BasicRule str_literal
            =   *(printable - L("\""));

    //文字列リテラル(ダブルクオーテーションを含む)
    BasicRule str_quoted
            =   L("\"") > str_literal > -L("\"");

    //文字列グループ
    BasicRule str_group
            =   L("(") >> str_expression >> L(")");

    //文字列値
    BasicRule str_func;
    BasicRule str_value
            =   str_func
            |   str_quoted
            |   str_array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"str_value")]
            |   str_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"str_value")]
            |   str_group;

    //文字列式
    str_expression
            =   str_value >> *(L("+") > str_value);

    //式
    BasicRule expression
            =   str_expression | num_expression;

    //変数
    StringRule var
            =   qi::raw[str_array_var | str_var | num_array_var | num_var];

    //関数----------------------------------------------------------------------
    //数値型関数
    //ASC
    BasicRule num_func_abs
            =   L("abs") >> L("(") > num_expression >> L(")");

    //ABS
    BasicRule num_func_asc
            =   L("asc") >> L("(") > str_expression >> L(")");

    //COS
    BasicRule num_func_cos
            =   L("cos") >> L("(") > num_expression >> L(")");

    //CSRLIN
    BasicRule num_func_csrlin
            =   L("csrlin");

    //CVS
    BasicRule num_func_cvs
            =   L("cvs") >> L("(") > str_expression >> L(")");

    //DEFFN文で定義された関数
    BasicRule num_func_deffn
            =   L("fn") >> num_var >> L("(") > num_expression >> L(")");

    //DSKF
    BasicRule num_func_dskf
            =   L("dskf") >> L("(") > num_expression
                          >> -(L(",") >> num_expression) >> L(")");

    //EOF
    BasicRule num_func_eof
            =   L("eof") >> L("(") > num_expression >> L(")");

    //ERL,ERR
    BasicRule num_func_erl
            =   L("erl");
    BasicRule num_func_err
            =   L("err");

    //EXP
    BasicRule num_func_exp
            =   L("exp") >> L("(") > num_expression >> L(")");

    //FRE
    BasicRule num_func_fre
            =   L("fre") >> L("(") >> expression >> L(")");

    //INP
    BasicRule num_func_inp
            =   L("inp") >> L("(") > num_expression >> L(")");

    //INT
    BasicRule num_func_int
            =   L("int") >> L("(") > num_expression >> L(")");

    //LEN
    BasicRule num_func_len
            =   L("len") >> L("(") > str_expression >> L(")");

    //LOC
    BasicRule num_func_loc
            =   L("loc") >> L("(") > num_expression >> L(")");

    //LOF
    BasicRule num_func_lof
            =   L("lof") >> L("(") > num_expression >> L(")");

    //LOG
    BasicRule num_func_log
            =   L("log") >> L("(") > num_expression >> L(")");

    //LPOS
    BasicRule num_func_lpos
            =   L("lpos") >> L("(") > num_expression >> L(")");

    //PAD
    BasicRule num_func_pad
            =   L("pad") >> L("(") > num_expression >> L(")");

    //PEEK
    BasicRule num_func_peek
            =   L("peek") >> num_expression;

    //POINT
    BasicRule num_func_point
            =   L("point") >> -L("step")
                           >> L("(") > num_expression >> L(",") >> num_expression >> L(")");

    //POS
    BasicRule num_func_pos
            =   L("pos") >> L("(") > num_expression >> L(")");

    //RND
    BasicRule num_func_rnd
            =   L("rnd") >> L("(") > num_expression >> L(")");

    //ROLL
    BasicRule num_func_roll
            =   L("roll") >> L("(") > num_expression >> L(")");

    //SCREEN
    BasicRule num_func_screen
            =   L("screen") >> L("(") > num_expression >> L(",") >> num_expression >> L(")");

    //SGN
    BasicRule num_func_sgn
            =   L("sgn") >> L("(") > num_expression >> L(")");

    //SIN
    BasicRule num_func_sin
            =   L("sin") >> L("(") > num_expression >> L(")");

    //SQR
    BasicRule num_func_sqr
            =   L("sqr") >> L("(") > num_expression >> L(")");

    //STICK
    BasicRule num_func_stick
            =   L("stick") >> L("(") > num_expression >> L(")");

    //STRIG
    BasicRule num_func_strig
            =   L("strig") >> L("(") > num_expression >> L(")");

    //TAN
    BasicRule num_func_tan
            =   L("tan") >> L("(") > num_expression >> L(")");

    //TIME
    BasicRule num_func_time
            =   L("time");

    //USR(数値を返す場合)
    //USR関数は引数に文字列を取るものと数字を取るものがあるため、
    //L("(") >> num_expressionのようにバックトラックを可能にしておく。
    BasicRule num_func_usr
            =   L("usr") >> L("(") >> num_expression >> L(")");

    //VAL
    BasicRule num_func_val
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
            |   num_func_err
            |   num_func_erl
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
    BasicRule str_func_chr$
            =   L("chr$") >> L("(") > num_expression >> L(")");

    //DSKI$
    BasicRule str_func_dski$
            =   L("dski$") >> L("(") > num_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression > L(")");

    //GRP$
    BasicRule str_func_grp$
            =   L("grp$") >> L("(") > num_expression >> L(")");

    //HEX$
    BasicRule str_func_hex$
            =   L("hex$") >> L("(") > num_expression >> L(")");

    //INKEY$
    BasicRule str_func_inkey$
            =   L("inkey$");

    //LEFT$
    BasicRule str_func_left$
            =   L("left$") >> L("(") > str_expression >> L(",") >> num_expression >> L(")");

    //MID$
    BasicRule str_func_mid$
            =   L("mid$") >> L("(") > str_expression
                          >> L(",") >> num_expression
                          >> -(L(",") >> num_expression)
                          >> L(")");

    //OCT$
    BasicRule str_func_oct$
            =   L("oct$") >> L("(") > num_expression >> L(")");

    //RIGHT$
    BasicRule str_func_right$
            =   L("right$") >> L("(") > str_expression >> L(",") >> num_expression >> L(")");
    //SPACE$
    BasicRule str_func_space$
            =   L("space$") >> L("(") > num_expression >> L(")");

    //STR$
    BasicRule str_func_str$
            =   L("str$") >> L("(") > num_expression >> L(")");

    //TIME$
    BasicRule str_func_time$
            =   L("time$") >> L("(") > num_expression >> L(")");

    //USR(文字列を返す場合)
    //USR関数は引数に文字列を取るものと数字を取るものがあるため、
    //L("(") >> str_expressionのようにバックトラックを可能にしておく。
    BasicRule str_func_usr
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
    BasicRule statement;
    //代入文
    BasicRule num_assign
            =   (num_array_var | num_var)[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"num_assign")]
            >>  L("=")
                > qi::as_wstring[*(char_ - lit(":"))]
                [phx::bind(&partial_parse, _1, ref(status), num_expression)];
    BasicRule str_assign
            =   (str_array_var | str_var)[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"str_assign")]
            >>  L("=") > str_expression;

    //グラフィック用２次元座標
    BasicRule coord_2d
            =   L("(") >> num_expression >> L(",") >> num_expression >> L(")");

    //AUTO文
    BasicRule st_auto
            =   L("auto") >> -linenumber >> -(L(",") > linenumber);

    //BGM文
    BasicRule st_bgm
            =   L("bgm") >> num_expression;

    //BLOAD文
    BasicRule st_bload
            =   L("bload") >> str_expression
                           >> -(L(",") >> -num_expression)
                           >> -(L(",") > -L("r"));
    //BSAVE文
    BasicRule st_bsave
            =   L("bsave") >> str_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression;

    //CIRCLE文
    BasicRule st_circle
            =   L("circle") >> -L("step") >> L("(")
                            >> num_expression >> L(",")     //x
                            >> num_expression >> L(")")     //y
                            >> L(",") >> num_expression     //r
                            >> -(L(",") >> -num_expression)   //c
                            >> -(L(",") >> -num_expression)   //s
                            >> -(L(",") >> -num_expression)   //e
                            >> -(L(",") > -num_expression);   //a

    //CLEAR文
    BasicRule st_clear
            =   L("clear") >> -num_expression
                           >> -(L(",") > num_expression);

    //CLOAD*文
    BasicRule st_cload_ast
            =   L("cload") >> L("*")
                           >> num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, true, L"st_cload_ast")];

    //CLOAD文(CLOAD?文も兼ねる)
    BasicRule st_cload
            =   L("cload") >> -L("?") >> -str_expression;

    //CLOSE文
    BasicRule st_close
            =   L("close") >> -(num_expression
                                >> *(L(",") >> num_expression));
    //CLS文
    BasicRule st_cls
            =   L("cls");

    //COLOR文
    BasicRule st_color
            =   L("color") >> -num_expression               //f
                           >> -(L(",") >> -num_expression)  //b
                           >> -(L(",") > -num_expression);  //c

    //CONSOLE文
    BasicRule st_console
            =   L("console") >> -num_expression               //m
                             >> -(L(",") >> -num_expression)    //n
                             >> -(L(",") >> -num_expression)    //f
                             >> -(L(",") >> -num_expression)    //k
                             >> -(L(",") > -num_expression);    //p

    //CONT文
    //プログラム中に存在し得ないはずだが、一応定義しておく。
    BasicRule st_cont
            =   L("cont");

    //CSAVE文
    BasicRule st_csave
            =   L("csave") >> str_expression;

    //CSAVE*文
    BasicRule st_csave_ast
            =   L("csave") >> L("*")
                           >> num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, true, L"st_csave_ast")];

    //DATA文
    BasicRule data_element
            =   +(printable - L("\"") - L(",") - L(":"))
            |   (L("\"") >> str_literal >> -L("\""));
    BasicRule st_data
            =   L("data") >> -data_element
                          >> *(L(",") > -data_element);

    //DEFFN文
    BasicRule st_deffn
            =   L("def") >> L("fn") >> num_var
                         >> L("(") >> num_var >> L(")") >> L("=")
                         >> num_expression;

    //DEFUSR文
    BasicRule st_defusr
            =   L("def") >> L("usr") >> L("=") >> num_expression;

    //DELETE文
    BasicRule st_delete
            =   L("delete") >> (linenumber || (L("-") >> -linenumber));

    //DIM文
    //#PENDING配列変数名の管理
    StringRule array_var
            =   qi::as_wstring[str_array_var | num_array_var];
    BasicRule st_dim
            =   L("dim") >> array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_dim")]
                         >> *(L(",") > array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_dim")]);

    //DSKO$文
    BasicRule st_dsko$
            =   L("dsko$") >> L("(") >> num_expression
                           >> L(",") >> num_expression
                           >> L(",") >> num_expression > L(")");

    //END文
    BasicRule st_end
            =   L("end");

    //ERASE文
    BasicRule st_erase
            =   L("erase") >> array_var
                           >> *(L(",") > array_var);

    //ERROR文
    BasicRule st_error
            =   L("error") >> num_expression;

    //EXEC文
    BasicRule st_exec
            =   L("exec") >> num_expression;

    //FIELD文
    BasicRule st_field
            =   L("field") >> -L("#")
                           >> num_expression
                           >> +(L(",")
                                >> qi::as_wstring[*(char_ - lit("as"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
                                >> L("as") >> str_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_field")]);

    //FILES文
    BasicRule st_files
            =   L("files") >> -num_expression;

    //FOR〜NEXT文
    //Spiritのルールは通常先頭から最長一致でマッチしてしまうため、
    //一部の構文に使われる予約語が変数名の一部として認識されてしまう。
    //例えば、forx=ytoz は、ytozが変数名として認識されてしまう。
    //苦肉の策として、for[任意の文字列]toという構文を定義しておき、
    //後からpartial_parse関数にて[任意の文字列]を数値式としてパースする。
    BasicRule st_for
            =   L("for") > num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_for")]
            >> L("=")
               > qi::as_wstring[*(char_ - lit("to"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
            >> L("to")
               > qi::as_wstring[*(char_ - lit("step") - lit(":"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
            >> -(L("step") >> num_expression);
    BasicRule st_next
            =   L("next") >> -(num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_next")]
                               >> *(L(",")
                                    > num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_next")]));

    //GET文
    BasicRule st_get
            =   L("get") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //GET@文
    BasicRule st_get_at
            =   L("get") >> -L("@") >> -L("step")
                         >> coord_2d
                         >> L("-") >> -L("step")
                         >> coord_2d
                         >> L(",") >> (str_expression | num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, true, L"st_get_at")]);

    //GOSUB〜RETURN文
    BasicRule st_gosub
            =   L("gosub") >> linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                           >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)];
    BasicRule st_return
            =   L("return");

    //GOTO文
    //PC-6000Techknowによると、「g o t o」でも通るため、トークンを分ける
    BasicRule st_goto
            =   L("g") >> L("o")  >> L("t") >> L("o") > linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                          >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)];

    //IF文
    BasicRule st_then
            =   (L("then") >> ((linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                                >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)]) | statement)
                 >> *(L(":") >> -statement))
            |   st_goto;
    BasicRule st_if
            =   L("if") >> qi::as_wstring[*(char_ - lit("then") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), logical_expression)]
                        >> qi::as_wstring[*(char_ - lit("else"))]
                           [phx::bind(&partial_parse, _1, ref(status), st_then)]
                        >> -((L("else") >> statement)
                            | (L("else") >> linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                               >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)]));

    //INPUT文
    BasicRule st_input
            =   L("input") >> -(str_expression >> L(";"))
                           >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input")]
                           >> *(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input")]);

    //INPUT#文
    BasicRule st_input_sharp
            =   L("input") >> L("#") >> num_expression
                           >> +(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input_sharp")]);
    //KANJI文
    BasicRule st_kanji
            =   L("kanji") >> -L("step")
                           >> coord_2d
                           >> L(",") >> num_expression //色
                           >> L(",") >> expression
                           >> *(-L(",") >> expression);
    //KEY文
    BasicRule st_key
            =   L("key") >> num_expression >> L(",") >> str_expression;

    //KILL文
    BasicRule st_kill
            =   L("kill") >> str_expression;

    //LCOPY文
    BasicRule st_lcopy
            =   L("lcopy") >> -num_expression;

    //LET文
    BasicRule st_let
            =   L("let") >> (str_assign | num_assign);

    //LFILES文
    BasicRule st_lfiles
            =   L("lfiles") >> num_expression;

    //LINE文
    BasicRule st_line
            =   L("line") >> -(-L("step")
                          >> coord_2d)
                          >> L("-") >> -L("step")
                          >> coord_2d
                          >> -(L(",") >> -num_expression) //色
                          >> -(L(",") >> (L("bf") | L("b")));

    //LIST文
    BasicRule st_list
            =   L("list") >> (linenumber || (L("-") >> -linenumber));

    //LIST L文
    BasicRule st_list_l
            =   L("list") >> L("l") >> -(L(",") >> linenumber);

    //LIST V文
    BasicRule st_list_v
            =   L("list") >> L("v") >> -(L(",")
                                         >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_list_v")]);

    //LLIST文
    BasicRule st_llist
            =   L("llist") >> (linenumber || (L("-") >> -linenumber));

    //LLIST L文
    BasicRule st_llist_l
            =   L("llist") >> L("l") >> -(L(",") >> linenumber);

    //LLIST V文
    BasicRule st_llist_v
            =   L("llist") >> L("v") >> -(L(",")
                                          >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_llist_v")]);

    //LOAD文
    BasicRule st_load
            =   L("load") >> str_expression >> -(L(",") >> L("r"));

    //LOCATE文(LOCATE@にも対応)
    BasicRule st_locate
            =   L("locate") >> -L("@") >> -num_expression
                          >> -(L(",") >> -num_expression)
                          >> -(L(",") >> num_expression);

    //LPRINT文
    //SPC,TAB関数はPRINT,LPRINT中でのみ使える。
    BasicRule str_func_spc
            =   L("spc") >> L("(") >> num_expression >> L(")");
    BasicRule str_func_tab
            =   L("tab") >> L("(") >> num_expression >> L(")");
    //PRINT対象文字列
    BasicRule str_print
            =   str_func_spc | str_func_tab | expression;
    BasicRule st_lprint
            =   (L("lprint")|L("?")) >> *((L(";") | L(",")) || str_print);

    //LSET文
    BasicRule st_lset
            =   L("lset") >> str_assign;

    //MENU文
    BasicRule st_menu
            =   L("menu");

    //MERGE文
    BasicRule st_merge
            =   L("merge") >> str_expression;

    //NAME文
    BasicRule st_name
            =   L("name") >> str_expression >> L("as") >> str_expression;

    //NEW文(こんなのプログラム中に有るのか？)
    BasicRule st_new
            =   L("new");

    //ON ERROR GOTO文
    BasicRule st_on_error_goto
            =   L("on") >> L("error") > st_goto;

    //ON GOSUB文
    BasicRule st_on_gosub
            =   L("on") >> !L("error")
                        >> qi::as_wstring[*(char_ - lit("gosub") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("gosub")
                        > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                        >> *(L(",") > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

    //ON GOTO文
    BasicRule st_on_goto
            =   L("on") >> !L("error")
                        >> qi::as_wstring[*(char_ - lit("goto") - lit("gosub"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("goto")
                        > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                        >> *(L(",") > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

    //OPEN文
    BasicRule st_open
            =   L("open") >> str_expression
                          >> -(L("for") >> (L("input") | L("output") | L("append")))
                          >> L("as") >> -L("#") >> num_expression;

    //OUT文
    BasicRule st_out
            =   L("out") >> num_expression >> L(",") >> num_expression;

    //PAINT文
    BasicRule st_paint
            =   L("paint") >> -L("step")
                           >> coord_2d
                           >> -(L(",") >> -num_expression) //領域色
                           >> -(L(",") >> num_expression); //境界色

    //PALET文
    BasicRule st_palet
            =   L("palet") >> -(num_expression >> L(",") >> num_expression);

    //PLAY文
    //#PENDING MML構文チェック
    BasicRule st_play
            =   L("play") >> -str_expression
                          >> qi::repeat(0, 4)[(L(",") >> -str_expression)]
                          >> -(L(",") >> str_expression);
    //POKE文
    BasicRule st_poke
            =   L("poke") >> num_expression >> L(",") >> num_expression;

    //PRESET文
    BasicRule st_preset
            =   L("preset") >> -L("step")
                            >> coord_2d;

    //PRINT文(PRINT@にも対応)
    BasicRule st_print
            =   (L("print")|L("?")) >> -L("@") >> *((L(";") | L(",")) || str_print);

    //PRINT#文
    BasicRule st_print_sharp
            =   L("print") >> L("#") >> num_expression
                           >> L(",") >> expression
                           >> *((L(";") | L(",")) > expression);

    //PSET文
    BasicRule st_pset
            =   L("pset") >> -L("step")
                          >> coord_2d
                          >> -(L(",") >> num_expression); //色
    //PUT文
    BasicRule st_put
            =   L("put") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //PUT@文
    BasicRule st_put_at
            =   L("put") >> -L("@") >> -L("step")
                         >> coord_2d
                         >> L(",") >> (num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, true, L"st_put_at")] | str_expression)
                         >> -(L(",") >> (L("xor") | L("and") | L("or") | L("pset") | L("preset"))); //色

    //READ文
    BasicRule st_read
            =   L("read") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_read")]
                         >> *(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_read")]);

    //REM文
    BasicRule st_rem
            =   (L("rem") | L("'")) >> *printable;

    //RENUM文
    BasicRule st_renum
            =   L("renum") >> -linenumber >> -(L(",") >> linenumber);

    //RESTORE文
    BasicRule st_restore
            =   L("restore") >> -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)];

    //RESUME文
    BasicRule st_resume
            =   L("resume") >> -(L("0") | L("next") | linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

    //ROLL文
    BasicRule st_roll
            =   L("roll") >> -num_expression
                          >> -(L(",") >> -num_expression)
                          >> -(L(",") >> L("y"));

    //RSET文
    BasicRule st_rset
            =   L("rset") >> str_assign;

    //RUN文
    BasicRule st_run
            =   L("run") >> -(linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                              |(str_expression >> -(L(",") >> L("r"))));

    //SAVE文
    BasicRule st_save
            =   L("save") >> str_expression >> -(L(",") >> L("a"));

    //SCREEN文
    BasicRule st_screen
            =   L("screen") >> -num_expression               //m
                            >> -(L(",") >> -num_expression)    //a
                            >> -(L(",") > num_expression);     //V

    //SOUND文
    BasicRule st_sound
            =   L("sound") >> num_expression >> L(",") >> num_expression;

    //STOP文
    BasicRule st_stop
            =   L("stop");

    //TALK文
    //#PENDING TALK文構文チェック
    BasicRule st_talk
            =   L("talk") >> str_expression;

    //TROFF文
    BasicRule st_troff
            =   L("troff");

    //TRON文
    BasicRule st_tron
            =   L("tron");

    //WAIT文
    BasicRule st_wait
            =   L("wait") >> num_expression
                          >> L(",") >> -num_expression
                          >> -(L(",") > num_expression);

    //WIDTH文
    BasicRule st_width
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
            |   str_assign
            |   num_assign;

    //行
    BasicRule line
            =   linenumber[phx::bind(&ParserStatus::registerLineNumber, ref(status), _1)]
            >   +(L(":") || statement);

    std::wstring::const_iterator first = program.begin();
    std::wstring::const_iterator last = program.end();

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

bool Checker::parse(const std::wstring& programList, ParserStatus& stat, bool trace)
{
    //作業用プログラムリスト
    std::wstring workProgramList = programList;
    //プリプロセス。
    //全角を半角に
    convZenHan(workProgramList);
    //大文字を小文字に
    transform(workProgramList.begin (), workProgramList.end (), workProgramList.begin (), tolower);

    //プログラムを行ごとに分割
    std::vector<std::wstring> list;
    boost::algorithm::split(list, workProgramList, boost::is_any_of(L"\n"));

    for(size_t i = 0; i < list.size(); i++){
        stat.inclementLine();
        const std::wstring line = list[i];
        if (line.empty()) continue;

        // 1行の構文解析結果を判定
        bool r = true;
        try{
            r = program_parse(line, stat);
        }
        catch (qi::expectation_failure<std::wstring::const_iterator> const& x)
        {
            if(trace){
                std::wcout << "expected: "; print_info(x.what_);
                std::wcout << "got: " << std::wstring(x.first, x.last) << std::endl;
                std::wcout << "textLine: " << stat.line_.textLineNumber_ << std::endl;
                std::wcout << "basicline: " << stat.line_.basicLineNumber_ << std::endl;
            }
            r = false;
        }
        if(!r){
            //出力の際は大文字に変換
            std::wstring workLine = line;
            transform(workLine.begin (), workLine.end (), workLine.begin (), toupper);

            stat.errorList_.push_back(ErrorInfo(E_SYNTAX, stat.line_.textLineNumber_, stat.line_.basicLineNumber_, (boost::wformat(L"シンタックスエラー[%1%]") % workLine).str()));
        }
    }

    //全行読み込み後のチェック
    afterCheck(stat);

    return stat.errorList_.empty();
}
