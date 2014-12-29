#ifndef WINDOW_H
#define WINDOW_H

#include <boost/noncopyable.hpp>
#include <ncursesw/ncurses.h>
#include "Vec.hpp"

class Window : public boost::noncopyable
{
public:

    Window(WINDOW * const new_win);
    Window(const Window &parentWindow, const Vec &pos, const Vec &size);

    virtual ~Window();

    Vec pos() const;
    Vec size() const;

    operator WINDOW * const() const;
private:
    WINDOW * const win;
};

#endif // WINDOW_H
