#ifndef CURSESSTREAM_H
#define CURSESSTREAM_H

#include "CursesStreamBuf.hpp"
#include <ostream>
#include <ncursesw/ncurses.h>
#include "Vec.hpp"
#include <utility>

class CursesStream : virtual private CursesStreamBuf, public std::ostream
{
public:
    CursesStream(WINDOW * const win);
    virtual ~CursesStream();

    template<typename T>
    CursesStream& operator<< (T&& val)
    {
        return static_cast<CursesStream&>(*static_cast<std::ostream*>(this) << std::forward<T>(val));
    }

    operator WINDOW * const() const;
private:
    WINDOW * const win;
};

template<>
inline CursesStream& CursesStream::operator<< (CursesStream& (&func)(CursesStream&))
{
    return func(*this);
}

class Move
{
public:
    Move(const Vec &new_pos) : pos(new_pos)
    {

    }

    friend CursesStream& operator<< (CursesStream &out, const Move &m)
    {
        wmove(out, m.pos.y, m.pos.x);
        return out;
    }

    friend CursesStream& operator<< (CursesStream &out, Move &&m)
    {
        wmove(out, m.pos.y, m.pos.x);
        return out;
    }
private:
    Vec pos;
};

class attrs
{
public:
    attrs(const chtype &new_ch) : ch(new_ch)
    {

    }

    friend CursesStream& operator<< (CursesStream &out, const attrs &at)
    {
        attrset(at.ch);
        return out;
    }

    friend CursesStream& operator<< (CursesStream &out, attrs &&at)
    {
        attrset(at.ch);
        return out;
    }
private:
    chtype ch;
};

inline CursesStream& normal(CursesStream &out)
{
    return out << attrs(A_NORMAL);
}

inline CursesStream& bold(CursesStream &out)
{
    return out << attrs(A_BOLD);
}

class Attron
{
public:
    Attron(const chtype &new_ch) : ch(new_ch)
    {

    }

    friend CursesStream& operator<< (CursesStream &out, const Attron &at)
    {
        attron(at.ch);
        return out;
    }

    friend CursesStream& operator<< (CursesStream &out, Attron &&at)
    {
        attron(at.ch);
        return out;
    }
private:
    chtype ch;
};

class Attroff
{
public:
    Attroff(const chtype &new_ch) : ch(new_ch)
    {

    }

    friend CursesStream& operator<< (CursesStream &out, const Attroff &at)
    {
        attroff(at.ch);
        return out;
    }

    friend CursesStream& operator<< (CursesStream &out, Attroff &&at)
    {
        attroff(at.ch);
        return out;
    }
private:
    chtype ch;
};

class Attrset
{
public:
    Attrset(const chtype &new_ch) : ch(new_ch)
    {

    }

    Attron operator+ () const
    {
        return Attron(ch);
    }

    Attroff operator- () const
    {
        return Attroff(ch);
    }

    friend CursesStream& operator<< (CursesStream &out, const Attrset &at)
    {
        attrset(at.ch);
        return out;
    }

    friend CursesStream& operator<< (CursesStream &out, Attrset &&at)
    {
        attrset(at.ch);
        return out;
    }
private:
    chtype ch;
};


extern Attrset Bold;


#endif // CURSESSTREAM_H
