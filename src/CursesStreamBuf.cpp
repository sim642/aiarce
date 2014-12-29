#include "CursesStreamBuf.hpp"

CursesStreamBuf::CursesStreamBuf(WINDOW * const new_win) : win(new_win)
{
    setp(nullptr, nullptr);
}

CursesStreamBuf::int_type CursesStreamBuf::overflow(int_type ch)
{
    waddch(win, ch);
    return traits_type::not_eof(ch);
}
