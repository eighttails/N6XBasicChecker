#include <QtCore/QString>
#include <QtTest/QtTest>
#include <string>
#include "checker.h"

class LibN6XBasicCheckerTest : public QObject
{
    Q_OBJECT

public:
    LibN6XBasicCheckerTest();

private:
    bool parse(const std::wstring& program, ParserStatus& stat);
private Q_SLOTS:
    void testCase1();

};

LibN6XBasicCheckerTest::LibN6XBasicCheckerTest()
{
}

bool LibN6XBasicCheckerTest::parse(const std::wstring& program, ParserStatus& stat)
{
    Checker checker;
    return checker.parse(program, stat);
}

void LibN6XBasicCheckerTest::testCase1()
{
    ParserStatus stat;
    std::wstring programList;
    //正常系
    programList =
            L"10 goto 10: go to10\n"
            "\n"
            "20 goto 20\n";
    QVERIFY(parse(programList, stat));

    //エラー行判定
    programList =
            L"10 goto 10: go to10\n"
            "20 goo 20\n"       //エラー
            "\n"
            "30 goto 30\n"
            "40 goto-40\n";     //エラー
    QVERIFY(!parse(programList, stat));
    QVERIFY(stat.errorList_.size() == 2);
    QVERIFY(stat.errorList_[0].textLineNumber_ ==  2);
    QVERIFY(stat.errorList_[0].basicLineNumber_ == 20);
    QVERIFY(stat.errorList_[1].textLineNumber_ ==  5);
    QVERIFY(stat.errorList_[1].basicLineNumber_ == 40);

    //PRINT文
    programList =
            L"10 print\"abcあいう\n"
            "20 print\"abcあいう\":goto10\n"
            "30 ?\"abcあいう\"\n";
    QVERIFY(parse(programList, stat));

    programList =
            L"10 print\"abcあいう\n"
            "20 print\"abcあいう\":got10\n"; //エラー
    QVERIFY(!parse(programList, stat));
    QVERIFY(stat.errorList_[0].basicLineNumber_ == 20);

}

QTEST_APPLESS_MAIN(LibN6XBasicCheckerTest);

#include "tst_libn6xbasiccheckertest.moc"
