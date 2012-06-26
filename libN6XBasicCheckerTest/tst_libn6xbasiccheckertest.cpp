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

    //正常系(各関数、ステートメント)
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
            "110 a=1:a=&h0a:as=1:to=1:a=1.0e-3:a=1e3\n"
            "120 b=abs(-1):b=abs(a)\n"
            "130 b=asc(\"abcあいう\"):b=asc(a$)\n"
            "140 auto:auto10:auto10,10:auto,10\n"
            "150 bgm 0:bgm1:bgma\n"
            "160 bloada$:bload\"a\",&ha000,r:bloada$,r:bloada$,&ha000\n"
            "170 bsavea$,&ha000,r\n"
            "180 a$=chr$(13)\n"
            "190 circle(x,y),r,c,s,e,a:circle step(x,y),r,c,,,a\n"
            "200 clear50:clear50,&hA000\n"
            "210 cload:cloada$:cload?\"abc\n"
            "220 cload * X\n"
            "230 close:close 1:close a,b,c:cls\n"
            "240 color1:color1,2,3:color,,3\n"
            "250 console1:console1,2,3,4,5:console1,,,,3:cont\n"
            "260 a=cos(1)\n"
            "270 csave\"aaa.bas\n"
            "280 csave * X\n"
            "290 a=csrlin\n"
            "300 a=cvs(a$)\n"
            "310 data1,2,3\n"
            "320 dataabc,def,ghi\n"
            "330 data\"abc def\",\"ghi jkl\n"
            "340 deffna(x)=2*x^2\n"
            "350 a=fna(x)\n"
            "360 def usr = &he000\n"
            "370 delete 370:delete 100-200:delete-150\n"
            "380 dima(5):dimb(a),c$(1,2,3),d(a,b,c)\n"
            "390 a=dskf(d):b=dskf(d,0)\n"
            "400 a$=dski$(a,b,c)\n"
            "410 dsko$(a,b,c)\n"
            "420 a=eof(0)\n"
            "430 end\n"
            "440 erasea(5):eraseb(a),c$(1,2,3),d(a,b,c)\n"
            "450 ERL=450:a=ERL:B=ERR\n"
            "460 error e:error 255\n"
            "470 exec &hA000\n"
            "480 a=exp(x)\n"
            "490 field#1,a*4asa$:field 1, aasa$,10 as b$\n"
            "500 files:files 1\n"
            "510 fori=0to10:next\n"
            "520 forj=xtoy:nextj\n"
            "530 fork=xtoystepy\n"
            "540 fork=xt+1toys+1stepy-1\n"
            "550 fre(0):fre(a$)\n"
            "560 get1,pr:get#1,pr\n"
            "570 get(0,0)-(15,15),d\n"
            "580 get@(0,0)-(15,15),d$\n"
            "590 getstep(0,0)-step(15,15),d\n"
            "600 get@step(0,0)-step(15,15),d$\n"
            "610 gosub 610:return\n"
            "620 a$=grp$(80)\n"
            "630 a$=hex$(i)\n"
            "640 ifa=5then640\n"
            "650 ifa=5thengoto640\n"
            "660 ifa=xgoto660\n"
            "670 ifa=xthena=10else670\n"
            "680 ifa=xthena=10elsea=100\n"
            "690 a$=inkey$\n"
            "700 a=inp(100)\n"
            "710 input a$:input \"input:\";a$,b$:input a$;b$(1)\n"
            "720 input #1,N,R:input #-1,N,R\n"
            "730 a=int(1.5)\n"
            "740 kanjistep(0,0),15,1000,\"あいう\",a,a$\n"
            "750 key1,a$\n"
            "760 kill a$\n"
            "770 lcopy2\n"
            "780 a$=left$(b$,2):a$=right$(b$,2)\n"
            "790 a=len(a$)\n"
            "800 leta=1:leta$=\"aaa\"\n"
            "810 lfiles2\n"
            "820 line(0,0)-(16,16)\n"
            "830 linestep(0,0)-step(16,16),1,b\n"
            "840 linestep(0,0)-step(16,16),,bf\n"
            "850 list 830:list 100-200:list-150\n"
            "860 list l,840\n"
            "870 list v,a:listv,a$\n"
            "880 llist 830:llist 100-200:llist-150\n"
            "890 llist l,840\n"
            "900 llist v,a:llistv,a$\n"
            "910 loada$:loada$,r\n"
            "920 a=loc(0)\n"
            "930 locate1,2:locate1,2,1:locate,,1\n"
            "940 a=lof(0)\n"
            "950 a=log(10)\n"
            "960 a=lpos(0)\n"
            "970 lprinta$;\"abcあいう\";b$;(a$+b$)\n"
            "980 lset a$=\"aaa\n"
            "990 rset a$=\"aaa\n"
            "1000 menu\n"
            "1010 merge f$\n"
            "1020 a$=mid$(b$,2):a$=mid$(b$,2,2)\n"
            "1030 name a$asb$\n"
            "1040 new\n"
            "1050 a$=oct$(i)\n"
            "1060 on error goto 1060\n"
            "1070 onagosub100,110,120\n"
            "1080 onagoto100,110,120\n"
            "1090 open f$as1:openf$as#1\n"
            "1100 open\"a.bas\"forappendas#1\n"
            "1110 out&h93,2\n"
            ;

    QVERIFY(parse(programList, stat, true));

    //エラー
    programList =
            "10 print\"abcあいう\":got10\n"
            "20 a=abs(\"aa\")\n"
            "30 a=asc(-a)\n"
            "40 auto,\n"
            "50 circle(x,y),,,,\n"
            "60 circle(x,y),r,c,s,e,a,a2\n"
            "70 delete\n"
            "80 locate,,"
            ;
    QVERIFY(!parse(programList, stat));
    int i=0;
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 10);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 20);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 30);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 40);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 50);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 60);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 70);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 80);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 90);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 100);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 110);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 120);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 130);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 140);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 150);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 160);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 170);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 180);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 190);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 200);
//    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 210);

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
            "200 COLORA$=B$:IFA$=\"A\"+\"B\"THEN110\n"
            "210 COLORA$=B$+\"A\"\n"
            "220 COLOR1+2:COLORA=B:COLORA$=\"AA\"\n"
            "230 IFA$<>B$THEN110:IFA$><B$THEN110:IFA$>B$THEN110:IFA$<B$THEN110\n"
            "240 IFA$>=B$THEN110:IFA$=>B$THEN110:IFA$<=B$THEN110:IFA$=<B$THEN110\n"
//            "300 COLOR(((A$=B$)+1)*2):PLAY(\"AA\"+\"BB\"):PLAYA$+(B$+C$):COLOR(A$=B$):COLOR(A$=\"AA\")\n"
//            "310 COLOR(\"AA\"=A$):COLOR(A$=\"AA\"):COLOR(INKEY$=\"AA\")\n"
            "320 A=(1):B=(-1):C=((1))\n"
            "330 COLOR(A$=B$+C$):COLOR(A$+B$=C$):COLOR(\"1\"+A$=B$)\n"
            "340 COLOR\"AA\"=A$:COLORA$=\"AA\":COLORINKEY$=\"AA\"\n"
            "350 COLORA$=B$+C$:COLORA$+B$=C$:COLOR\"1\"+A$=B$\n"
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
    int i=0;
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 500);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 510);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 520);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 530);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 540);
    //QVERIFY(stat.errorList_[i++].basicLineNumber_ == 550); エミュレータでエラーにならなかったので、テストケースから除外
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 560);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 570);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 580);
    QVERIFY(stat.errorList_[i++].basicLineNumber_ == 590);

}

QTEST_APPLESS_MAIN(LibN6XBasicCheckerTest)

#include "tst_libn6xbasiccheckertest.moc"
