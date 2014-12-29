#ifndef CURSESSTREAMBUF_HPP
#define CURSESSTREAMBUF_HPP

#include <streambuf>
#include <ncursesw/ncurses.h>
#include <boost/noncopyable.hpp>

class CursesStreamBuf : public std::streambuf, public boost::noncopyable
{
public:
    explicit CursesStreamBuf(WINDOW * const new_win);

private:
    int_type overflow(int_type ch);
public:
    WINDOW * const win;
};

#endif // CURSESSTREAMBUF_HPP
