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

extern"C"{
#include "txt2bas.h"
}

int main(int argc, char *argv[])
{
    babel::init_babel();

    po::options_description desc("オプション", 200);
    desc.add_options()
            ("help,h", "ヘルプを表示")
            ("version,v", "バージョンを表示")
            ("play,p", po::value<std::vector<std::string> >(), "PLAY文としてパースする行")
            ("talk,t", po::value<std::vector<std::string> >(), "TALK文としてパースする行")
            ("hex,x", po::value<std::vector<std::string> >(), "16進数としてパースする行")
            ("digit,d", po::value<std::vector<std::string> >(), "10進数の整数(符号付き)としてパースする行")
            ("output-mode,b", po::value<int>()->default_value(5), "テープイメージ出力モード(5または6。省略時は5)")
            ("output-file,o", po::value<std::string>(), "テープイメージ出力ファイル")
            ("basic-file,f", po::value<std::string>()->default_value("t2b"), "テープイメージ内のBASICリストファイル名")
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
    //構文エラー
    bool argError = false;
    try{
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).positional(p).run(), vm);
    po::notify(vm);
    } catch (...){
        argError = true;
    }


    //バージョン情報
    if (vm.count("version")) {
        std::cout << "N6XBasicChecker ver." << Checker::version() << std::endl
                  << "Copyright 2012-2014 Tadahito Yao(@eighttails)" << std::endl
                  << "http://eighttails.seesaa.net" << std::endl;
        return 0;
    }

    //ヘルプオプションが指定、またはファイル名が指定されていない場合はヘルプを表示
    if (vm.count("help") || !vm.count("input-file") || argError) {
        std::cout << utf8_to_local("Usage: N6XBasicChecker ファイル名 [オプション]") << std::endl;
        std::stringstream s;
        s << desc;
        std::cout << utf8_to_local(s.str());
        std::cout << utf8_to_local("p,t,x,dオプションの行番号指定例: -p 100,200-300") << std::endl;
        std::cout << utf8_to_local("b,o,fオプションの書式: -b 5 -o 出力ファイル名 -f BASICリストファイル名") << std::endl;
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
    //10進数としてパースする行
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

            //txt2bas呼び出し
            if(vm.count("output-mode") && vm.count("output-file")){
                int outMode = vm["output-mode"].as<int>();
                if(outMode != 5 && outMode != 6){
                    std::cout << utf8_to_local("bオプションの書式が間違っています。") << std::endl;
                    return -1;
                }
                std::string outFile = vm["output-file"].as<std::string>();
                int srmode = 0;
                if(outMode == 6) srmode = 1;

                std::string basFile = vm["basic-file"].as<std::string>();

                std::cout << utf8_to_local("テープイメージ出力: ") << outFile << std::endl;
                ret &= txt2bas_main(srmode, const_cast<char*>(fileName.c_str()), const_cast<char*>(outFile.c_str()), const_cast<char*>(basFile.c_str())) ? false : true;
                std::cout << utf8_to_local("Ok") << std::endl;
            }
        }
    }

    return ret ? 0 : -1;
}

