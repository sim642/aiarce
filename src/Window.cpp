#include "Window.hpp"

//Window::Window(WINDOW * const new_win) : stream(new_win), win(new_win)
Window::Window(WINDOW * const new_win) : CursesStream(new_win), win(new_win)
{

}

Window::Window(const Vec &pos, const Vec &size) : Window(newwin(size.y, size.x, pos.y, pos.x))
{

}

Window::Window(const Window &parentWindow, const Vec &pos, const Vec &size) : Window(derwin(parentWindow.win, size.y, size.x, pos.y, pos.x))
{

}

Window::~Window()
{
    delwin(win);
}

Vec Window::pos() const
{
    Vec ret;
    getparyx(win, ret.y, ret.x);
    if (ret == Vec{-1, -1})
        getyx(win, ret.y, ret.x);
    return ret;
}

Vec Window::size() const
{
    Vec ret;
    getmaxyx(win, ret.y, ret.x);
    return ret;
}

int Window::length() const
{
    Vec ret = size();
    return ret.x * ret.y;
}

Window::operator WINDOW * const() const
{
    return win;
}

/*Window::operator CursesStream&() const
{
    return stream;
}*/
