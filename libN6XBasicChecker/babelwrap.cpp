#include "babelwrap.h"
#include "babel.cpp"

bool utf8Output = defaultUtf8Output;

const std::string utf8_to_local(const std::string &str)
{
	if (utf8Output) {
		return str;
	} else {
		return babel::utf8_to_sjis(str);
	}
}

const std::string unicode_to_local(const std::wstring &str)
{
	if (utf8Output) {
		return babel::unicode_to_utf8(str);
	} else {
		return babel::unicode_to_sjis(str);
	}
}

const std::wstring local_to_unicode(const std::string &str)
{
	if (utf8Output){
		return babel::utf8_to_unicode(str);
	} else {
		return babel::sjis_to_unicode(str);
	}
}
