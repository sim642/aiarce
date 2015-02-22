#ifndef WINDOW_H
#define WINDOW_H

#include <boost/noncopyable.hpp>
#include <ncursesw/ncurses.h>
#include "Vec.hpp"
#include "CursesStream.hpp"

class Window : public CursesStream, virtual public boost::noncopyable
{
public:
    Window(WINDOW * const new_win);
    Window(const Vec &pos, const Vec &size);
    Window(const Window &parentWindow, const Vec &pos, const Vec &size);

    virtual ~Window();

    Vec pos() const;
    Vec size() const;
    int length() const;

    operator WINDOW * const() const;
    //operator CursesStream& () const;
private:
    WINDOW * const win;
    //CursesStream stream;
};

#endif // WINDOW_H
