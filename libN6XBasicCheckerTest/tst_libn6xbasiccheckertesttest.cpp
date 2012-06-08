#include <QtCore/QString>
#include <QtTest/QtTest>
#include <string>
#include "checker.h"

class LibN6XBasicCheckerTestTest : public QObject
{
    Q_OBJECT

public:
    LibN6XBasicCheckerTestTest();

private:
    bool parse(const std::wstring& program, ParserStatus& stat);
private Q_SLOTS:
    void testCase1();

};

LibN6XBasicCheckerTestTest::LibN6XBasicCheckerTestTest()
{
}

bool LibN6XBasicCheckerTestTest::parse(const std::wstring& program, ParserStatus& stat)
{
    Checker checker;
    return checker.parse(program, stat);
}

void LibN6XBasicCheckerTestTest::testCase1()
{
    ParserStatus stat;
    std::wstring str =
            L"10 goto 10: go to10\n"
            "20 goto 20\n";
    QVERIFY(parse(str, stat));
    str = L"10 goto 10: go to10\n"
            "20 goo 20\n"
            "30 goto 30\n";
    QVERIFY(!parse(str, stat));
    QCOMPARE(stat.textLineNumber_,2);
}

QTEST_APPLESS_MAIN(LibN6XBasicCheckerTestTest);

#include "tst_libn6xbasiccheckertesttest.moc"
