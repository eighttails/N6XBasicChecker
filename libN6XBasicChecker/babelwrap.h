#ifndef BABELWRAP_H
#define BABELWRAP_H

//babelに関連するヘッダーおよびマクロ
#include "babel.h"
#include <string>

#if 0
#ifdef WIN32
#define utf8_to_local(a) babel::utf8_to_sjis(a)
#define unicode_to_local(a) babel::unicode_to_sjis(a)
#define local_to_unicode(a) babel::sjis_to_unicode(a)
#else
#define utf8_to_local(a) a
#define unicode_to_local(a) babel::unicode_to_utf8(a)
#define local_to_unicode(a) babel::utf8_to_unicode(a)
#endif
#endif // if 0

const std::string utf8_to_local(const std::string& str);
const std::string unicode_to_local(const std::wstring& str);
const std::wstring local_to_unicode(const std::string& str);

#endif // BABELWRAP_H
