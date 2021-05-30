#ifndef BABELWRAP_H
#define BABELWRAP_H

//babelに関連するヘッダーおよびマクロ
#include "babel.h"
#include <string>

#ifdef WIN32
const bool defaultUtf8Output = false;
#else
const bool defaultUtf8Output = true;
#endif

extern bool utf8Output;

const std::string utf8_to_local(const std::string& str);
const std::string unicode_to_local(const std::wstring& str);
const std::wstring local_to_unicode(const std::string& str);

#endif // BABELWRAP_H
