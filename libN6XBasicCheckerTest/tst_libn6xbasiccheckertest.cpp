#include <QtCore/QString>
#include <QtTest/QtTest>
#include <string>
#include <iostream>
#include "checker.h"

class LibN6XBasicCheckerTest : public QObject
{
    Q_OBJECT

public:
    LibN6XBasicCheckerTest();

private:
    bool parse(const std::string& program, ParserStatus& stat, bool trace = false);
private Q_SLOTS:
    void testCase1();
    void testCase2();

};

LibN6XBasicCheckerTest::LibN6XBasicCheckerTest()
{
}

bool LibN6XBasicCheckerTest::parse(const std::string& program, ParserStatus& stat, bool trace)
{
    Checker checker;
    bool r = checker.parse(program, stat, trace);
    if(!r && trace){
        for(size_t i = 0; i < stat.errorList_.size(); i++){
            const ErrorInfo& err = stat.errorList_[i];
            std::cout << "テキスト行:" << err.textLineNumber_
                      << " BASIC行:" << err.basicLineNumber_
                      << " " << err.info_ << std::endl;
        }
    }
    return r;
}

void LibN6XBasicCheckerTest::testCase1()
{
    ParserStatus stat;
    std::string programList;
    //正常系
    programList =
            "10 goto 10: go to10\n"
            "\n"
            "20 goto 20\n"
            ;
    QVERIFY(parse(programList, stat, true));

    //エラー行判定
    programList =
            "10 goto 10: go to10\n"
            "20 goo 20\n"       //エラー
            "\n"
            "30 goto 30\n"
            "40 goto-40\n"     //エラー
            ;
    QVERIFY(!parse(programList, stat));
    QVERIFY(stat.errorList_.size() == 2);
    QVERIFY(stat.errorList_[0].textLineNumber_ ==  2);
    QVERIFY(stat.errorList_[0].basicLineNumber_ == 20);
    QVERIFY(stat.errorList_[1].textLineNumber_ ==  5);
    QVERIFY(stat.errorList_[1].basicLineNumber_ == 40);

    //PRINT文
    programList =
            "10 print\"abcあいう\n"
            "20 print\"abcあいう\":goto10\n"
            "30 ?\"abcあいう\"\n"
            "40 printabcde $:printabcde\n"
            "50 printa$;\"abcあいう\";b$;(a$+b$)\n"
            "60 printa$(1);b$(1,2,3),\"abcあいう\n"
            "70 print(a$=\"1\"):print(a+b=1)\n"
            "80 printnot(a$=\"1\"):printnot-a=1:print-a\n"
            "90 printa and b:printa or b:print a xor b\n"
            "100 a$=\"1\"\n"
            "110 a=1\n"
            "120 b=abs(-1):b=abs(a)\n"
            "130 b=asc(\"abcあいう\"):b=asc(a$)\n"
            "140 auto:auto10:auto10,10:auto,10\n"
            "150 bgm 0:bgm1:bgma\n"
            ;
    QVERIFY(parse(programList, stat, true));

    programList =
            "10 print\"abcあいう\":got10\n"
            "20 a=abs(\"aa\")\n"
            "30 a=asc(-a)\n"
            "40 auto,\n"
            ;
    QVERIFY(!parse(programList, stat));
    int i=0;
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 10);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 20);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 30);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 40);

}
void LibN6XBasicCheckerTest::testCase2()
{
    //えすびさんに提供してもらったテストケース
    //モード4までの対応とのことだが、構文系のテストとして利用させていただきます。

    ParserStatus stat;
    std::string programList;
    //正常系
    programList =
//            "110 A=1:A$=\"11\":C=EXP(1):D$=MID$(INKEY$,1,2)\n"
//            "120 A$=\"11\"+\"33\":B$=INKEY$+LEFT$(\"aa\",1)\n"
//            "130 A$=\"1\"+\"a\"+\"BB\"+LEFT$(\"AA\",1)+MID$(B$,1,2)+\"11\"\n"
//            "140 A=1+3+B+CSRLIN+STICK(0)\n"
//            "150 A=1+VAL(\"a\")+VAL(\"BB\")+SIN(1)+MO(1,2,3)+11\n"
            "160 A=1=2:A=1<>2:A=1><2:A=1>2:A=1<2:A=1>=2:A=1=>2:A=1<=2:A=1=<2\n"
            "170 A=1^2:A=1-2:A=1*2:A=1/2:A=1+2:A=1AND2:A=1OR2\n"
            "180 A=NANDA:B=NORS\n"
            "190 A=-1+2:B=-1.2*2+1:C=+111*2:D=NOTA+NOTB\n"
//            "200 COLORA$=B$:IFA$=\"A\"+\"B\"THEN110\n"
//            "210 COLORA$=B$+\"A\"\n"
//            "220 COLOR1+2:COLORA=B:COLORA$=\"AA\"\n"
//            "230 IFA$<>B$THEN110:IFA$><B$THEN110:IFA$>B$THEN110:IFA$<B$THEN110\n"
//            "240 IFA$>=B$THEN110:IFA$=>B$THEN110:IFA$<=B$THEN110:IFA$=<B$THEN110\n"
//            "300 COLOR(((A$=B$)+1)*2):PLAY(\"AA\"+\"BB\"):PLAYA$+(B$+C$):COLOR(A$=B$):COLOR(A$=\"AA\")\n"
//            "310 COLOR(\"AA\"=A$):COLOR(A$=\"AA\"):COLOR(INKEY$=\"AA\")\n"
            "320 A=(1):B=(-1):C=((1))\n"
//            "330 COLOR(A$=B$+C$):COLOR(A$+B$=C$):COLOR(\"1\"+A$=B$)\n"
//            "340 COLOR\"AA\"=A$:COLORA$=\"AA\":COLORINKEY$=\"AA\"\n"
//            "350 COLORA$=B$+C$:COLORA$+B$=C$:COLOR\"1\"+A$=B$\n"
            ;
    QVERIFY(parse(programList, stat, true));

    //異常系
    programList =
            "500 A$=\"1\",\"b\":B$=\"AA\"+\n"
            "510 A=1+:B=111TO\n"
            "520 COLORA$=B$+1:COLORB$+1=A$\n"
            "530 COLORA$=B$*1:COLORB$*1=A$\n"
            "540 A=A$=\"!!\":B=1+\"AA\"=B$:C=1+B$=\"BB\"\n"
            "550 A=A$<>B$:B=B$<\"A\"\n"
            "560 A=(1:B=1):C=(1\n"
            "570 D=):E=()\n"
            "580 COLOR(\"AA\"<>A):COLOR(A$><1):COLOR(INKEY$<=1)\n"
            "590 COLOR1+A$=B$\n"
            ;
    QVERIFY(!parse(programList, stat));
}

QTEST_APPLESS_MAIN(LibN6XBasicCheckerTest);

#include "tst_libn6xbasiccheckertest.moc"
