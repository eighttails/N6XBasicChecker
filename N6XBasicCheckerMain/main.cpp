#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "babel.cpp"

#include "checker.h"

#ifdef WIN32
#define utf8_to_local(a) babel::utf8_to_sjis(a)
#else
#define utf8_to_local(a) a
#endif

int main(int argc, char *argv[])
{
    po::options_description desc("オプション");
    desc.add_options()
            ("help,h", "ヘルプを表示")
            ("version,v", "バージョンを表示")
            //("play,p", po::value<std::vector<std::string> >(), "PLAY文としてパースする行")
            ;
    po::options_description hidden("不可視オプション");
    hidden.add_options()
            ("input-file", po::value<std::string>(), "input file")
            ;

    //無名のオプションはファイル名として処理される
    po::positional_options_description p;
    p.add("input-file", -1);

    po::options_description cmdline_options;
    cmdline_options.add(desc).add(hidden);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).positional(p).run(), vm);
    po::notify(vm);


    //バージョン情報
    if (vm.count("version")) {
        std::cout << "N6XBasicChecker ver." << Checker::version() << std::endl
                  << "Copyright 2012 Tadahito Yao(@eighttails)" << std::endl
                  << "http://eighttails.seesaa.net" << std::endl;
        return 0;
    }

    //ヘルプオプションが指定、またはファイル名が指定されていない場合はヘルプを表示
    if (vm.count("help") || !vm.count("input-file")) {
        std::cout << utf8_to_local("Usage: N6XBasicChecker ファイル名 [オプション]") << std::endl;
        std::stringstream s;
        s << desc;
        std::cout << utf8_to_local(s.str());
        return 0;
    }


    // コマンドライン引数から変数に取り込み
    if(vm.count("input-file")){
        //ファイル読み込み
        std::string fileName = vm["input-file"].as<std::string>();
        std::ifstream fst;
        fst.open(fileName.c_str());
        if(fst.fail()){
            std::cout << utf8_to_local("ファイルのオープンに失敗しました。") << std::endl;
            return -1;
        }
        std::string sjisList((std::istreambuf_iterator<char>(fst)), std::istreambuf_iterator<char>());
        std::string utf8List = babel::sjis_to_utf8(sjisList);

        //パース実行
        Checker checker;
        ParserStatus stat;
        bool r = checker.parse(utf8List, stat);

        //エラー表示
        if(!stat.errorList_.empty()){
            std::cout << utf8_to_local("エラー:") << std::endl;
        }
        for(size_t i = 0; i < stat.errorList_.size(); i++){
            const ErrorInfo& err = stat.errorList_[i];
            std::cout << utf8_to_local("テキスト行:") << err.textLineNumber_
                      << utf8_to_local(" BASIC行:") <<
                         ((err.basicLineNumber_ == -1) ? utf8_to_local("N/A") : boost::lexical_cast<std::string>(err.basicLineNumber_))
                      << " " << utf8_to_local(err.info_) << std::endl;
        }
        std::cout << utf8_to_local("Ok") << std::endl;

        return r ? 0 : -1;
    }

    return 0;
}

