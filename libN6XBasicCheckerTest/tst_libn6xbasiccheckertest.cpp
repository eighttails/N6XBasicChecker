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
    //正常系
    std::wstring str =
            L"10 goto 10: go to10\n"
            "20 goto 20\n";
    QVERIFY(parse(str, stat));

    //エラー行判定
    str = L"10 goto 10: go to10\n"
            "20 goo 20\n"
            "30 goto 30\n";
    QVERIFY(!parse(str, stat));
    QVERIFY(stat.errorList_.size() == 1);
    QVERIFY(stat.errorList_[0].textLineNumber_ ==  2);
    QVERIFY(stat.errorList_[0].basicLineNumber_ == 20);
}

QTEST_APPLESS_MAIN(LibN6XBasicCheckerTest);

#include "tst_libn6xbasiccheckertest.moc"
