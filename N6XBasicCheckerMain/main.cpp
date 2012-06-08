#include <QtCore/QCoreApplication>
#include <QTextCodec>

//static void reportError	(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer)
//{
//    pANTLR3_LEXER lexer   = (pANTLR3_LEXER)(recognizer->super);
//    printf("文法エラーです。 テキスト行:%d BASIC行:%d\n",
//            lexer->rec->state->tokenStartLine,
//            status_.currentBasicLine_);
//}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//    char data[] = {"10 print\"aaa\":'日\r\n" "20 rem あいうえお\r\n"};
    QString data = "10 ?\"aaa日\"\n20 goto30\n20 goto30\n";


    return 0;
}

