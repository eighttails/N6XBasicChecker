#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "parserstatus.h"
#include "checker.h"

#include "babelwrap.h"


int main(int argc, char *argv[])
{
    babel::init_babel();

    po::options_description desc("オプション");
    desc.add_options()
            ("help,h", "ヘルプを表示")
            ("version,v", "バージョンを表示")
            ("play,p", po::value<std::vector<std::string> >(), "PLAY文としてパースする行")
            ("talk,t", po::value<std::vector<std::string> >(), "TALK文としてパースする行")
            ("hex,x", po::value<std::vector<std::string> >(), "16進数としてパースする行")
            ("digit,d", po::value<std::vector<std::string> >(), "10進数の整数(符号付き)としてパースする行")
            ;
    po::options_description hidden("不可視オプション");
    hidden.add_options()
            ("input-file", po::value<std::vector<std::string> >(), "input file")
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
        std::cout << utf8_to_local("p,t,x,dオプションの行番号指定例: -p100,200-300") << std::endl;
        return 0;
    }

    bool ret = true;
    ParserStatus stat;

    //PLAY文としてパースする行
    if(vm.count("play")){
        std::vector<std::string> linesList = vm["play"].as<std::vector<std::string> >();
        for (size_t i = 0; i < linesList.size(); i++){
            std::string lines = linesList[i];
            if(!stat.registerLineRange(local_to_unicode(lines), R_PLAY)){
                std::cout << utf8_to_local("pオプションの書式が間違っています。") << std::endl;
                return -1;
            }
        }
    }
    //TALK文としてパースする行
    if(vm.count("talk")){
        std::vector<std::string> linesList = vm["talk"].as<std::vector<std::string> >();
        for (size_t i = 0; i < linesList.size(); i++){
            std::string lines = linesList[i];
            if(!stat.registerLineRange(local_to_unicode(lines), R_TALK)){
                std::cout << utf8_to_local("tオプションの書式が間違っています。") << std::endl;
                return -1;
            }
        }
    }
    //16進数としてパースする行
    if(vm.count("hex")){
        std::vector<std::string> linesList = vm["hex"].as<std::vector<std::string> >();
        for (size_t i = 0; i < linesList.size(); i++){
            std::string lines = linesList[i];
            if(!stat.registerLineRange(local_to_unicode(lines), R_HEX)){
                std::cout << utf8_to_local("xオプションの書式が間違っています。") << std::endl;
                return -1;
            }
        }
    }
    //16進数としてパースする行
    if(vm.count("digit")){
        std::vector<std::string> linesList = vm["digit"].as<std::vector<std::string> >();
        for (size_t i = 0; i < linesList.size(); i++){
            std::string lines = linesList[i];
            if(!stat.registerLineRange(local_to_unicode(lines), R_DIGIT)){
                std::cout << utf8_to_local("dオプションの書式が間違っています。") << std::endl;
                return -1;
            }
        }
    }
    // コマンドライン引数から変数に取り込み
    if(vm.count("input-file")){
        //ファイル読み込み
        std::vector<std::string> fileNames = vm["input-file"].as<std::vector<std::string> >();
        if(fileNames.size() != 1){
            std::cout << utf8_to_local("ファイル名は１つのみ指定可能です。") << std::endl;
            return -1;
        }
        for (size_t i = 0; i < fileNames.size(); i++){
            std::ifstream fst;
            std::string fileName = fileNames[i];
            std::cout << utf8_to_local("解析中: ") << fileName << std::endl;
            fst.open(fileName.c_str());
            if(fst.fail()){
                std::cout << utf8_to_local("ファイルのオープンに失敗しました。") << std::endl;
                return -1;
            }
            std::string sjisList((std::istreambuf_iterator<char>(fst)), std::istreambuf_iterator<char>());
            std::wstring unicodeList = babel::sjis_to_unicode(sjisList);

            //パース実行
            Checker checker;
            bool r = checker.parse(unicodeList, stat);

            //エラー表示
            if(!stat.errorList_.empty()){
                std::cout << utf8_to_local("エラー:") << std::endl;
            }
            for(size_t i = 0; i < stat.errorList_.size(); i++){
                const ErrorInfo& err = stat.errorList_[i];
                std::cout << utf8_to_local("テキスト行:") << err.line_.textLineNumber_
                          << utf8_to_local(" BASIC行:") <<
                             ((err.line_.basicLineNumber_ == -1) ? utf8_to_local("N/A") : boost::lexical_cast<std::string>(err.line_.basicLineNumber_))
                          << " " << unicode_to_local(err.info_) << std::endl;
            }
            //警告表示
            if(!stat.warningList_.empty()){
                std::cout << utf8_to_local("警告:") << std::endl;
            }
            for(size_t i = 0; i < stat.warningList_.size(); i++){
                const ErrorInfo& err = stat.warningList_[i];
                std::cout << utf8_to_local("テキスト行:") << err.line_.textLineNumber_
                          << utf8_to_local(" BASIC行:") <<
                             ((err.line_.basicLineNumber_ == -1) ? utf8_to_local("N/A") : boost::lexical_cast<std::string>(err.line_.basicLineNumber_))
                          << " " << unicode_to_local(err.info_) << std::endl;
            }
            std::cout << utf8_to_local("Ok") << std::endl;

            ret &= r;
        }
    }

    return ret ? 0 : -1;
}

