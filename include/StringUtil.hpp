#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>

std::wstring to_wide(const std::string &str);
std::string from_wide(const std::wstring &wstr);

#endif // STRINGUTIL_H
