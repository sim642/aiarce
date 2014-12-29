#include <clocale>
#include <ncursesw/ncurses.h>
#include "Window.hpp"
#include "Input.hpp"
#include <iostream>
#include "CursesStream.hpp"
#include <cstring>

using namespace std;

int main()
{
    setlocale(LC_ALL, "");
    initscr();

    Window mainWindow(stdscr);
    Window inputWindow(mainWindow, {1, 10}, {15, 2});

    Input::line_func = [&](const string &line)
        {
            CursesStream out(mainWindow);
            out << Move({0, 1}) << "<" << +Bold << line << -Bold << ">";

            wclrtoeol(mainWindow);

            mvwprintw(mainWindow, 0, 0, "<%s>", line.c_str());
            wclrtoeol(mainWindow);
            wrefresh(mainWindow);
        };
    Input::setup(&inputWindow);

    Vec s = mainWindow.size();

    int i = 0;
    while (1)
    {
        Input::handle();

        mvwprintw(mainWindow, 4, 0, "%d %d - %d", s.x, s.y, i++);
        wclrtoeol(mainWindow);
        wrefresh(mainWindow);

        Input::fixCursor();

        usleep(1000 * 1);
    }

    endwin();
    return 0;
}
