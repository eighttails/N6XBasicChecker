#include <iostream>
#include <locale>
#include <locale.h>
#include "babel.cpp"


//#define utf8_to_local(a) babel::utf8_to_sjis(a)
//#define unicode_to_local(a) babel::unicode_to_sjis(a)
//#define local_to_unicode(a) babel::sjis_to_unicode(a)

const std::string utf8_to_local(const std::string &str)
{
	printf("%s\n", setlocale(LC_ALL, NULL));
//	auto global_locale = std::locale("");
//	std::locale global_locale("");
//	std::cout << global_locale.name() << std::endl;
	if (1){
		return babel::utf8_to_sjis(str);
	} else {
		return babel::utf8_to_sjis(str);

	}
}

const std::string unicode_to_local(const std::wstring &str)
{
	return babel::unicode_to_sjis(str);
}

const std::wstring local_to_unicode(const std::string &str)
{
	return babel::sjis_to_unicode(str);
}
