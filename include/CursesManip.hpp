#ifndef CURSESMANIP_H
#define CURSESMANIP_H

#include "CursesStream.hpp"

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

inline CursesStream& Refresh(CursesStream &out)
{
    wrefresh(out);
    return out;
}

inline CursesStream& ClrToEol(CursesStream &out)
{
    int x, y;
    getyx(out, y, x);
    if (x + 1 < getmaxx(out)) // don't clear of blocked
        wclrtoeol(out);
    return out;
}

inline CursesStream& ClrToBot(CursesStream &out)
{
    int x, y, maxx, maxy;
    getyx(out, y, x);
    getmaxyx(out, maxy, maxx);
    if (y + 1 < maxy || (y + 1 == maxy && x + 1 < maxx)) // don't clear of blocked
        wclrtobot(out);
    return out;
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


extern Attrset Bold, Normal;


#endif // CURSESMANIP_H
