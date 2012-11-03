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
bool partial_parse(std::wstring const& part, ParserStatus& status, StringRule const& rule)
{
    std::wstring::const_iterator first = part.begin();
    std::wstring::const_iterator last = part.end();

    bool r = qi::phrase_parse(first, last, rule, sw::blank);

    if (!r || first != last) {
        //出力の際は大文字に変換
        std::wstring workPart = part;
        transform(workPart.begin (), workPart.end (), workPart.begin (), toupper);

        status.errorList_.push_back(ErrorInfo(E_PART_SYNTAX, status.line_.textLineNumber_, status.line_.basicLineNumber_, (boost::wformat(L"部分シンタックスエラー[%1%]") % workPart).str()));
        return false;
    }

    return true;
}

//PLAY文の構文チェック
bool play_parse(std::wstring const& part, ParserStatus& status, StringRule const& num_expression)
{
    //ルール記述
    //値
    StringRule mml_value
            =   uint_
            |   (L("=") >> num_expression >> L(";"));

    //音長
    StringRule mml_length
            =   uint_ || L(".");

    //コマンド
    //@コマンド
    StringRule mml_at
            =   L("@") >> -mml_value;
    //Oコマンド
    StringRule mml_o
            =   L("o") >> -mml_value;
    //Tコマンド
    StringRule mml_t
            =   L("t") >> -(mml_length | mml_value);
    //Lコマンド
    StringRule mml_l
            =   L("l") >> -(mml_length | mml_value);
    //Rコマンド
    StringRule mml_r
            =   L("r") >> -(mml_length | mml_value);
    //Vコマンド
    StringRule mml_v
            =   L("v") >> -mml_value;
    //Sコマンド
    StringRule mml_s
            =   L("s") >> -mml_value;
    //Mコマンド
    StringRule mml_m
            =   L("m") >> -mml_value;
    //Nコマンド
    StringRule mml_n
            =   L("n") >> -mml_value
                       >> -mml_length;

    //音符
    StringRule mml_note
            =   (L("c") | L("d") | L("e") | L("f") | L("g") | L("a") | L("b"))
            >> -(L("+") | L("-") | L("#"))
            >> -uint_
            >> -L(".");

    StringRule mml_element
            =   (mml_note
                 | mml_at
                 | mml_o
                 | mml_t
                 | mml_l
                 | mml_r
                 | mml_v
                 | mml_s
                 | mml_m
                 | mml_n);

    StringRule mml
        =   *(mml_element >> -L(";"));

    std::wstring::const_iterator first = part.begin();
    std::wstring::const_iterator last = part.end();

    bool r = qi::phrase_parse(first, last, mml, sw::blank);

    if (!r || first != last) {
        //出力の際は大文字に変換
        std::wstring workPart = part;
        transform(workPart.begin (), workPart.end (), workPart.begin (), toupper);

        status.errorList_.push_back(ErrorInfo(E_PLAY, status.line_.textLineNumber_, status.line_.basicLineNumber_, (boost::wformat(L"MMLエラー[%1%]") % workPart).str()));
        return false;
    }

    return true;
}

//TALK文の構文チェック
bool talk_parse(std::wstring const& part, ParserStatus& status, StringRule const& num_expression){
    //ルール記述
    //性別
    StringRule talk_sex
            =   (L("m") | L("f"));

    //速度
    StringRule talk_speed
            =   (L("1") | L("2") | L("3") | L("4") | L("5") | L("6") | L(" "));

    //固定語
    StringRule talk_fixed
            =  talk_sex
            >> talk_speed
            >> (L("0") | L("1") | L("2") | L("3") | L("4"));

    //音節
    NoSkipStringRule talk_syllable
            =   L("kya") | L("kyu") | L("kye") | L("kyo")
            |   L("sya") | L("syu") | L("sye") | L("syo")
            |   L("cya") | L("cyu") | L("cye") | L("cyo")
            |   L("nya") | L("nyu") | L("nye") | L("nyo")
            |   L("hya") | L("hyu") | L("hye") | L("hyo")
            |   L("mya") | L("myu") | L("mye") | L("myo")
            |   L("rya") | L("ryu") | L("rye") | L("ryo")
            |   L("gya") | L("gyu") | L("gye") | L("gyo")
            |   L("zya") | L("zyu") | L("zye") | L("zyo")
            |   L("bya") | L("byu") | L("bye") | L("byo")
            |   L("pya") | L("pyu") | L("pye") | L("pyo")
            |   L("ka") | L("ki") | L("ku") | L("ke") | L("ko")
            |   L("sa") | L("si") | L("su") | L("se") | L("so")
            |   L("ta") | L("ci") | L("cu") | L("te") | L("to")
            |   L("na") | L("ni") | L("nu") | L("ne") | L("no")
            |   L("ha") | L("hi") | L("hu") | L("he") | L("ho")
            |   L("ma") | L("mi") | L("mu") | L("me") | L("mo")
            |   L("ya")           | L("yu")           | L("yo")
            |   L("ra") | L("ri") | L("ru") | L("re") | L("ro")
            |   L("wa")
            |   L("ga") | L("gi") | L("gu") | L("ge") | L("go")
            |   L("za") | L("zi") | L("zu") | L("ze") | L("zo")
            |   L("da") | L("di") | L("du") | L("de") | L("do")
            |   L("ba") | L("bi") | L("bu") | L("be") | L("bo")
            |   L("pa") | L("pi") | L("pu") | L("pe") | L("po")
            |   L("fa") | L("fi") | L("fu") | L("fe") | L("fo")
            |   L("ti") | L("tu") | L("x") | L("q")
            |   L("a") | L("i") | L("u") | L("e") | L("o")
            ;

    //修飾付き音節
    StringRule talk_syllable_decorated
            =  -(L("^") || L("+"))
            >> talk_syllable
            >> -(L("'") ^ L("-"))
            >> -(L("*") ^ L("?"));

    //音声文字列
    StringRule talk_string
            =  talk_syllable_decorated
            >> *(-L("/") >> talk_syllable_decorated);

    StringRule talk
            =  (talk_fixed
                | (talk_sex
                   >> -talk_speed
                   >> -(L("+") | L("-") | L("#"))
                   >> talk_string))
            >> -(L(".") | L("?"))
            >> -(L(":") > qi::as_wstring[*char_][phx::bind(&play_parse, _1, ref(status), num_expression)]);

    std::wstring::const_iterator first = part.begin();
    std::wstring::const_iterator last = part.end();

    bool r = qi::phrase_parse(first, last, talk, sw::blank);

    if (!r || first != last) {
        //出力の際は大文字に変換
        std::wstring workPart = part;
        transform(workPart.begin (), workPart.end (), workPart.begin (), toupper);

        status.errorList_.push_back(ErrorInfo(E_TALK, status.line_.textLineNumber_, status.line_.basicLineNumber_, (boost::wformat(L"TALK文エラー[%1%]") % workPart).str()));
        return false;
    }
    return true;
}

//任意の文字列リテラルが、明示的にPLAY文、TALK分としてチェックするよう指定されている場合にチェックを行う。
//PLAY文の一部にnum_expressionを使ってパースする箇所があるため、ここまで引き回す。
bool literal_parse(std::wstring const& part, ParserStatus& status, StringRule const& num_expression){
    int line = status.line_.basicLineNumber_;

    //PLAY文チェック
    bool playRange = false;
    for (ParserStatus::LineRange::iterator p = status.playRange_.begin(); p != status.playRange_.end(); ++p){
        if(p->first <= line && line <= p->second) playRange = true;
    }
    if (status.playMode_ || playRange){
        if (!play_parse(part, status, num_expression)) return false;
    }

    //TALK文チェック
    bool talkRange = false;
    for (ParserStatus::LineRange::iterator p = status.talkRange_.begin(); p != status.talkRange_.end(); ++p){
        if(p->first <= line && line <= p->second) talkRange = true;
    }
    if (status.talkMode_ || talkRange){
        if (!talk_parse(part, status, num_expression)) return false;
    }

    //16進数チェック
    for (ParserStatus::LineRange::iterator p = status.hexRange_.begin(); p != status.hexRange_.end(); ++p){
        if(p->first <= line && line <= p->second){
            std::wstring::const_iterator first = part.begin();
            std::wstring::const_iterator last = part.end();

            bool r = qi::phrase_parse(first, last, *(sw::xdigit), sw::blank);

            if (!r || first != last) {
                //出力の際は大文字に変換
                std::wstring workPart = part;
                transform(workPart.begin (), workPart.end (), workPart.begin (), toupper);

                status.errorList_.push_back(ErrorInfo(E_HEX, status.line_.textLineNumber_, status.line_.basicLineNumber_, (boost::wformat(L"16進数エラー[%1%]") % workPart).str()));
                return false;
            }
            break;
        }
    }

    return true;
}

bool program_parse(const std::wstring& program, ParserStatus& status)
{
    //グラフィック文字
    NoSkipStringRule graph
            =   L("月")|L("火")|L("水")|L("木")|L("金")|L("土")|L("日")|L("年")|L("円")
            |	L("時")|L("分")|L("秒")|L("百")|L("千")|L("万")|L("π")
            |	L("┻")|L("┳")|L("┫")|L("┣")|L("╋")|L("┃")|L("━")|L("┏")|L("┓")|L("┗")|L("┛")
            |	L("×")|L("大")|L("中")|L("小")|L("▲")|L("▼")|L("★")|L("◆")|L("○")|L("●");

    //かな記号
    NoSkipStringRule kana_kigou
            =	L("「")|L("」")|L("、")|L("。")|L("・")|L("゛")|L("゜")|L("ー");

    //ひらがな
    NoSkipStringRule hiragana
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
    NoSkipStringRule katakana
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
    NoSkipStringRule han_kana
            =	L("｡")|L("｢")|L("｣")|L("､")|L("･")|L("ｰ")
            |   L("ｦ")|L("ｧ")|L("ｨ")|L("ｩ")|L("ｪ")|L("ｫ")|L("ｬ")|L("ｭ")|L("ｮ")|L("ｯ")
            |	L("ｱ")|L("ｲ")|L("ｳ")|L("ｴ")|L("ｵ")|L("ｶ")|L("ｷ")|L("ｸ")|L("ｹ")|L("ｺ")
            |	L("ｻ")|L("ｼ")|L("ｽ")|L("ｾ")|L("ｿ")|L("ﾀ")|L("ﾁ")|L("ﾂ")|L("ﾃ")|L("ﾄ")
            |	L("ﾅ")|L("ﾆ")|L("ﾇ")|L("ﾈ")|L("ﾉ")|L("ﾊ")|L("ﾋ")|L("ﾌ")|L("ﾍ")|L("ﾎ")
            |	L("ﾏ")|L("ﾐ")|L("ﾑ")|L("ﾒ")|L("ﾓ")|L("ﾔ")|L("ﾕ")|L("ﾖ")
            |	L("ﾗ")|L("ﾘ")|L("ﾙ")|L("ﾚ")|L("ﾛ")|L("ﾜ")|L("ﾝ")
            |   L("ﾞ")|L("ﾟ");

    //使用可能な文字
    NoSkipStringRule printable
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
            =   qi::raw[sw::alpha >> qi::repeat(0, 4)[sw::alnum - reserved]];

    //数値型変数(配列)
    StringRule num_array_var
            =   qi::raw[num_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")")];

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
            |   num_array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"num_value")]
            |   num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"num_value")]
            |   num_group;

    //単項式
    StringRule unary_expression
            =   (L("-") >> num_value)
            |   (L("not") >> num_value)
            |   num_value;

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
            =   qi::raw[sw::alpha >> qi::repeat(0, 4)[sw::alnum] >> L("$")];

    //文字列変数(配列)
    StringRule str_array_var
            =   qi::raw[str_var >> L("(") > num_expression >> *(L(",") > num_expression) > L(")")];

    //文字列リテラル(ダブルクオーテーションを含まない)
    //2つ目のダブルクオートの前に「-」が付いているのは、行末のダブルクオートは省略できるという仕様への対応
    NoSkipStringRule str_literal
            =   qi::as_wstring[*(printable - lit("\""))][phx::bind(&literal_parse, _1, ref(status), num_expression)];

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
            |   str_array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"str_value")]
            |   str_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"str_value")]
            |   str_group;

    //文字列式
    str_expression
            =   str_value >> *(L("+") > str_value);

    //式
    StringRule expression
            =   str_expression | num_expression;

    //変数
    StringRule var
            =   qi::raw[str_array_var | str_var | num_array_var | num_var];

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
    StringRule num_func_erl
            =   L("erl");
    StringRule num_func_err
            =   L("err");

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

    //STRING$
    StringRule str_func_string$
            =   L("string$") >> L("(") > num_expression >> L(",") >> (str_expression | num_expression) >> L(")");

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
            |   str_func_string$
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
            =   (num_array_var | num_var)[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"num_assign")]
            >>  L("=")
                > qi::as_wstring[*(printable - lit(":"))]
                [phx::bind(&partial_parse, _1, ref(status), num_expression)];
    StringRule str_assign
            =   (str_array_var | str_var)[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"str_assign")]
            >>  L("=") > str_expression;

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
    StringRule st_cload_ast
            =   L("cload") >> L("*")
                           >> num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, true, L"st_cload_ast")];

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
            =   L("csave") >> L("*")
                           >> num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, true, L"st_csave_ast")];

    //DATA文
    StringRule data_element
            =   qi::as_wstring[+(printable - L("\"") - L(",") - L(":"))][phx::bind(&literal_parse, _1, ref(status), num_expression)]
            |   (L("\"") >> qi::as_wstring[str_literal][phx::bind(&literal_parse, _1, ref(status), num_expression)] >> -L("\""));
    StringRule st_data
            =   L("data") >> -data_element
                          >> *(L(",") > -data_element);

    //DEFFN文
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
            =   qi::as_wstring[str_array_var | num_array_var];
    StringRule st_dim
            =   L("dim") >> array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_dim")]
                         >> *(L(",") > array_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_dim")]);

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
                                >> qi::as_wstring[*(char_ - lit("as"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
                                >> L("as") >> str_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_field")]);

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
            =   L("for") > num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_for")]
            >> L("=")
               > qi::as_wstring[*(char_ - lit("to"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
            >> L("to")
               > qi::as_wstring[*(char_ - lit("step") - lit(":"))][phx::bind(&partial_parse, _1, ref(status), num_expression)]
            >> -(L("step") >> num_expression);
    StringRule st_next
            =   L("next") >> -(num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_next")]
                               >> *(L(",")
                                    > num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_next")]));

    //GET文
    StringRule st_get
            =   L("get") >> -L("#") >> num_expression
                         >> -(L(",") >> num_expression);

    //GET@文
    StringRule st_get_at
            =   L("get") >> -L("@") >> -L("step")
                         >> coord_2d
                         >> L("-") >> -L("step")
                         >> coord_2d
                         >> L(",") >> (str_expression | num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, true, L"st_get_at")]);

    //GOSUB〜RETURN文
    StringRule st_gosub
            =   L("gosub") >> linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                           >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)];
    StringRule st_return
            =   L("return");

    //GOTO文
    //PC-6000Techknowによると、「g o t o」でも通るため、トークンを分ける
    StringRule st_goto
            =   L("g") >> L("o")  >> L("t") >> L("o") > linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                          >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)];

    //IF文
    StringRule st_then
            =   (L("then") >> ((linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                                >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)]) | statement)
                 >> *(L(":") >> -statement))
            |   st_goto;
    StringRule st_if
            =   L("if") >> qi::as_wstring[*(char_ - lit("then") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), logical_expression)]
                        >> qi::as_wstring[*(char_ - lit("else"))]
                           [phx::bind(&partial_parse, _1, ref(status), st_then)]
                        >> -((L("else") >> statement)
                            | (L("else") >> linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                               >> -qi::as_wstring[+(char_ - lit(":"))][phx::bind(&ParserStatus::warnRedundantContent, ref(status), _1)]));

    //INPUT文
    StringRule st_input
            =   L("input") >> -(str_expression >> L(";"))
                           >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input")]
                           >> *(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input")]);

    //INPUT#文
    StringRule st_input_sharp
            =   L("input") >> L("#") >> num_expression
                           >> +(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_input_sharp")]);
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
            =   L("list") >> L("v") >> -(L(",")
                                         >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_list_v")]);

    //LLIST文
    StringRule st_llist
            =   L("llist") >> (linenumber || (L("-") >> -linenumber));

    //LLIST L文
    StringRule st_llist_l
            =   L("llist") >> L("l") >> -(L(",") >> linenumber);

    //LLIST V文
    StringRule st_llist_v
            =   L("llist") >> L("v") >> -(L(",")
                                          >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, false, L"st_llist_v")]);

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
            =   L("tab") >> L("(") >> num_expression >> L(")");
    //PRINT対象文字列
    StringRule str_print
            =   str_func_spc | str_func_tab | expression;
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
                        >> qi::as_wstring[*(char_ - lit("gosub") - lit("goto"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("gosub")
                        > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                        >> *(L(",") > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

    //ON GOTO文
    StringRule st_on_goto
            =   L("on") >> !L("error")
                        >> qi::as_wstring[*(char_ - lit("goto") - lit("gosub"))]
                           [phx::bind(&partial_parse, _1, ref(status), num_expression)]
                        >> L("goto")
                        > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
                        >> *(L(",") > -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

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
    StringRule st_play
            =   L("play")[ref(status.playMode_) = true]
            >>  -str_expression
            >>  qi::repeat(0, 4)[(L(",") >> -str_expression)]
            >>  -(L(",") >> str_expression)
            >>  qi::eps[ref(status.playMode_) = false];

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
                         >> L(",") >> (num_var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_REFER, true, L"st_put_at")] | str_expression)
                         >> -(L(",") >> (L("xor") | L("and") | L("or") | L("pset") | L("preset"))); //色

    //READ文
    StringRule st_read
            =   L("read") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_read")]
                         >> *(L(",") >> var[phx::bind(&ParserStatus::registerUsedVariable, ref(status), _1, VAR_ASSIGN, false, L"st_read")]);

    //REM文
    StringRule st_rem
            =   (L("rem") | L("'")) >> *printable;

    //RENUM文
    StringRule st_renum
            =   L("renum") >> -linenumber >> -(L(",") >> linenumber);

    //RESTORE文
    StringRule st_restore
            =   L("restore") >> -linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)];

    //RESUME文
    StringRule st_resume
            =   L("resume") >> -(L("0") | L("next") | linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]);

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
            =   L("run") >> -(linenumber[phx::bind(&ParserStatus::registerReferredLineNumber, ref(status), _1)]
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
            =  L("talk")[ref(status.talkMode_) = true]
            >> str_expression
            >> *(L(",") || str_expression)
            >> qi::eps[ref(status.talkMode_) = false];

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
            |   str_assign
            |   num_assign;

    //行
    StringRule line
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

void Checker::makeZenHanMap()
{
    //全角半角テーブル
    zenhanMap_[L"　"] = L" ";
    zenhanMap_[L"！"] = L"!";
    zenhanMap_[L"”"] = L"\"";
    zenhanMap_[L"＃"] = L"#";
    zenhanMap_[L"＄"] = L"$";
    zenhanMap_[L"％"] = L"%";
    zenhanMap_[L"＆"] = L"&";
    zenhanMap_[L"’"] = L"'";
    zenhanMap_[L"（"] = L"(";
    zenhanMap_[L"）"] = L")";
    zenhanMap_[L"＊"] = L"*";
    zenhanMap_[L"＋"] = L"+";
    zenhanMap_[L"，"] = L",";
    zenhanMap_[L"－"] = L"-";
    zenhanMap_[L"．"] = L".";
    zenhanMap_[L"／"] = L"/";
    zenhanMap_[L"："] = L":";
    zenhanMap_[L"；"] = L";";
    zenhanMap_[L"＜"] = L"<";
    zenhanMap_[L"＝"] = L"=";
    zenhanMap_[L"＞"] = L">";
    zenhanMap_[L"？"] = L"?";
    zenhanMap_[L"＠"] = L"@";
    zenhanMap_[L"〔"] = L"[";
    zenhanMap_[L"［"] = L"[";
    zenhanMap_[L"￥"] = L"\\";
    zenhanMap_[L"〕"] = L"]";
    zenhanMap_[L"］"] = L"]";
    zenhanMap_[L"＾"] = L"^";
    zenhanMap_[L"＿"] = L"_";
    zenhanMap_[L"｛"] = L"{";
    zenhanMap_[L"｜"] = L"|";
    zenhanMap_[L"｝"] = L"}";
    zenhanMap_[L"〜"] = L"~";

    zenhanMap_[L"０"] = L"0";
    zenhanMap_[L"１"] = L"1";
    zenhanMap_[L"２"] = L"2";
    zenhanMap_[L"３"] = L"3";
    zenhanMap_[L"４"] = L"4";
    zenhanMap_[L"５"] = L"5";
    zenhanMap_[L"６"] = L"6";
    zenhanMap_[L"７"] = L"7";
    zenhanMap_[L"８"] = L"8";
    zenhanMap_[L"９"] = L"9";

    zenhanMap_[L"Ａ"] = L"A";
    zenhanMap_[L"ａ"] = L"a";
    zenhanMap_[L"Ｂ"] = L"B";
    zenhanMap_[L"ｂ"] = L"b";
    zenhanMap_[L"Ｃ"] = L"C";
    zenhanMap_[L"ｃ"] = L"c";
    zenhanMap_[L"Ｄ"] = L"D";
    zenhanMap_[L"ｄ"] = L"d";
    zenhanMap_[L"Ｅ"] = L"E";
    zenhanMap_[L"ｅ"] = L"e";
    zenhanMap_[L"Ｆ"] = L"F";
    zenhanMap_[L"ｆ"] = L"f";
    zenhanMap_[L"Ｇ"] = L"G";
    zenhanMap_[L"ｇ"] = L"g";
    zenhanMap_[L"Ｈ"] = L"H";
    zenhanMap_[L"ｈ"] = L"h";
    zenhanMap_[L"Ｉ"] = L"I";
    zenhanMap_[L"ｉ"] = L"i";
    zenhanMap_[L"Ｊ"] = L"J";
    zenhanMap_[L"ｊ"] = L"j";
    zenhanMap_[L"Ｋ"] = L"K";
    zenhanMap_[L"ｋ"] = L"k";
    zenhanMap_[L"Ｌ"] = L"L";
    zenhanMap_[L"ｌ"] = L"l";
    zenhanMap_[L"Ｍ"] = L"M";
    zenhanMap_[L"ｍ"] = L"m";
    zenhanMap_[L"Ｎ"] = L"N";
    zenhanMap_[L"ｎ"] = L"n";
    zenhanMap_[L"Ｏ"] = L"O";
    zenhanMap_[L"ｏ"] = L"o";
    zenhanMap_[L"Ｐ"] = L"P";
    zenhanMap_[L"ｐ"] = L"p";
    zenhanMap_[L"Ｑ"] = L"Q";
    zenhanMap_[L"ｑ"] = L"q";
    zenhanMap_[L"Ｒ"] = L"R";
    zenhanMap_[L"ｒ"] = L"r";
    zenhanMap_[L"Ｓ"] = L"S";
    zenhanMap_[L"ｓ"] = L"s";
    zenhanMap_[L"Ｔ"] = L"T";
    zenhanMap_[L"ｔ"] = L"t";
    zenhanMap_[L"Ｕ"] = L"U";
    zenhanMap_[L"ｕ"] = L"u";
    zenhanMap_[L"Ｖ"] = L"V";
    zenhanMap_[L"ｖ"] = L"v";
    zenhanMap_[L"Ｗ"] = L"W";
    zenhanMap_[L"ｗ"] = L"w";
    zenhanMap_[L"Ｘ"] = L"X";
    zenhanMap_[L"ｘ"] = L"x";
    zenhanMap_[L"Ｙ"] = L"Y";
    zenhanMap_[L"ｙ"] = L"y";
    zenhanMap_[L"Ｚ"] = L"Z";
    zenhanMap_[L"ｚ"] = L"z";

    zenhanMap_[L"\r\n"] = L"\n";

}

void Checker::replaceString( std::wstring& str, const std::wstring& from, const std::wstring& to )
{
    std::wstring::size_type  p( str.find( from ) );

    while( p != std::wstring::npos )
    {
        str.replace( p, from.length(), to );
        p = str.find( from, p + to.length() );
    }
}

void Checker::convZenHan(std::wstring& programList)
{
    //全角半角テーブルに従って全角文字を半角に置換
    std::map<std::wstring, std::wstring>::const_iterator p;
    for (p = zenhanMap_.begin(); p != zenhanMap_.end(); ++p){
        replaceString(programList, p->first, p->second);
    }
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

void Checker::afterCheck(ParserStatus& stat)
{
    //行番号チェック
    for (size_t i = 0; i < stat.ReferredLineNumberList_.size(); i++){
        const ReferredLineNumber& ref = stat.ReferredLineNumberList_[i];
        if(stat.basicLineNumberList_.count(ref.targetLineNumber_) == 0){
            stat.errorList_.push_back(ErrorInfo(E_LINE_NOT_FOUND, ref.refererLine_.textLineNumber_, ref.refererLine_.basicLineNumber_,
                                                (boost::wformat(L"BASIC行%1%は存在しません") % ref.targetLineNumber_).str()));
        }
    }

    //変数チェック
    std::map<std::wstring, std::map<std::wstring, UsedVar> >::const_iterator i;
    std::map<std::wstring, UsedVar>::const_iterator j;
    std::set<VarLineInfo>::const_iterator k;

    for(i = stat.usedVariables_.begin(); i != stat.usedVariables_.end(); ++i){
        const std::map<std::wstring, UsedVar>& subMap = (*i).second;
        for(j = subMap.begin(); j != subMap.end(); ++j){
            const UsedVar& var = (*j).second;

            //警告出力の際は大文字に変換
            std::wstring workIdent = var.identName_;
            transform(workIdent.begin (), workIdent.end (), workIdent.begin (), toupper);
            std::wstring workVarName = var.varName_;
            transform(workVarName.begin (), workVarName.end (), workVarName.begin (), toupper);

            //識別子重複チェック(2文字目までが一致しているが、違う名前が使われている変数)
            if(subMap.size() > 1){
                for(k = var.assigningLines_.begin(); k != var.assigningLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).line_.textLineNumber_, (*k).line_.basicLineNumber_,
                                                          (boost::wformat(L"[%1%]として識別される変数が複数存在します[%2%]") % workIdent % workVarName).str()));
                }
                for(k = var.referingLines_.begin(); k != var.referingLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_DUPLICATE_VARIABLE, (*k).line_.textLineNumber_, (*k).line_.basicLineNumber_,
                                                          (boost::wformat(L"[%1%]として識別される変数が複数存在します[%2%]") % workIdent % workVarName).str()));
                }
            }

            //参照されているが代入されていない変数
            if(var.assigningLines_.empty()){
                for(k = var.referingLines_.begin(); k != var.referingLines_.end(); ++k){
                    stat.warningList_.push_back(ErrorInfo(W_UNASSIGNED_VARIABLE, (*k).line_.textLineNumber_, (*k).line_.basicLineNumber_,
                                                          (boost::wformat(L"変数[%1%]はどこからも代入されていません") % workVarName).str()));
                }
            }

            //代入されているが参照されていない変数
            if(var.referingLines_.empty()){
                for(k = var.assigningLines_.begin(); k != var.assigningLines_.end(); ++k){
                    if((*k).ruleName_ != L"st_for"){ //forのループカウンタとして使われている場合は警告を出さない
                        stat.warningList_.push_back(ErrorInfo(W_UNREFERED_VARIABLE, (*k).line_.textLineNumber_, (*k).line_.basicLineNumber_,
                                                              (boost::wformat(L"変数[%1%]はどこからも参照されていません") % workVarName).str()));
                    }
                }
            }
        }
    }

    //エラー、警告をソート
    std::stable_sort(stat.errorList_.begin(), stat.errorList_.end());
    std::stable_sort(stat.warningList_.begin(), stat.warningList_.end());

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

