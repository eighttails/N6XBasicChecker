#include <boost/program_options.hpp>
#include <string>

using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using boost::program_options::store;
using boost::program_options::parse_command_line;
using boost::program_options::notify;

#include "babel.cpp"
#ifdef WIN32
#define utf8_to_local(a) babel::utf8_to_sjis(a)
#else
#define utf8_to_local(a) a
#endif



int main(int argc, char *argv[])
{
    options_description opt("オプション");
    // 引数の書式を定義
    opt.add_options()
        ("help,h", "ヘルプを表示")
        ("address,a", value<std::string>(), "マルチキャストアドレス")
        ("port,p", value<int>(), "ポート番号")
        ("interface,i", value<std::string>(), "インタフェース名");

    // 引数の書式に従って実際に指定されたコマンドライン引数を解析
    variables_map argmap;
    store(parse_command_line(argc, argv, opt), argmap);
    notify(argmap);

    // ヘルプ表示指定があるか、必須引数が抜けていた場合、ヘルプ表示して終了
    if (argmap.count("help") || !argmap.count("address") || !argmap.count("port")) {
        std::cerr << utf8_to_local(opt) << std::endl;
        return 1;
    }

    // コマンドライン引数から変数に取り込み
    std::string address = argmap["address"].as<std::string>();
    int port = argmap["port"].as<int>();
    const char* interface = 0;
    if (argmap.count("interface")) {
        interface = argmap["interface"].as<std::string>().c_str();
    }

    return 0;
}

