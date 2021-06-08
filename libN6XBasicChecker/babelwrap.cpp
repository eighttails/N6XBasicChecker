#include <iostream>
#include <stdarg.h>
#include <string.h>

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


int printf_local(const char *format, ...)
{
    char buf[1024];
    va_list ap;
    va_start( ap, format );
    vsprintf( buf, format, ap );
    std::cout << utf8_to_local(buf);
	std::cout.flush();
    va_end( ap );
    return strlen(buf);
}

int fprintf_local(FILE *fp, const char *format, ...)
{
    char buf[1024];
    va_list ap;
    va_start( ap, format );
    int size = vsprintf( buf, format, ap );
    fwrite(buf, size, 1, fp);
    va_end( ap );
    return strlen(buf);
}
