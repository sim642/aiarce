#include "Window.hpp"

Window::Window(WINDOW * const new_win) : win(new_win)
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
    return ret;
}

Vec Window::size() const
{
    Vec ret;
    getmaxyx(win, ret.y, ret.x);
    return ret;
}

Window::operator WINDOW * const() const
{
    return win;
}
