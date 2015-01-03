#include "StringUtil.hpp"
#include <cwchar>

std::wstring to_wide(const std::string &str)
{
    const char *p = str.c_str(); // necessary to copy because mbsrtowcs fucks around
    std::mbstate_t state = std::mbstate_t();
    int len = std::mbsrtowcs(nullptr, &p, 0, &state);
    std::wstring wstr(len, L'\0');
    std::mbsrtowcs(&wstr[0], &p, wstr.size(), &state);
    return wstr;
}

std::string from_wide(const std::wstring &wstr)
{
    const wchar_t *p = wstr.c_str(); // necessary to copy because wcsrtombs fucks around
    std::mbstate_t state = std::mbstate_t();
    int len = std::wcsrtombs(nullptr, &p, 0, &state);
    std::string str(len, '\0');
    std::wcsrtombs(&str[0], &p, str.size(), &state);
    return str;
}
