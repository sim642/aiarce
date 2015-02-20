#include <clocale>
#include <ncursesw/ncurses.h>
#include "Window.hpp"
#include "Input.hpp"
#include <iostream>
#include "CursesStream.hpp"
#include "CursesManip.hpp"
#include <cstring>
#include <csignal>
#include <sys/ioctl.h>
#include <cstdio>
#include <unistd.h>

using namespace std;

int i;

void resize_handler(int)
{
    signal(SIGWINCH, SIG_IGN);
    i++;

    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    resize_term(w.ws_row, w.ws_col);
    refresh();

    signal(SIGWINCH, resize_handler);
}

int main()
{
    setlocale(LC_ALL, "");
    initscr();

    signal(SIGWINCH, resize_handler);

    Window mainWindow(stdscr);
    /*Window outerWindow(mainWindow, {3, 9}, {17, 4});
    Window inputWindow(outerWindow, {1, 1}, {15, 2});*/
    Window outerWindow(mainWindow, {3, 9}, {17, 4});
    Window inputWindow(outerWindow, {1, 1}, {15, 2});
    //scrollok(inputWindow, TRUE);

    box(outerWindow, '|', '-');
    //wprintw(outerWindow, "----");
    //touchwin((WINDOW*)mainWindow);
    outerWindow << Refresh;
    //touchwin((WINDOW*)outerWindow);
    //wrefresh(mainWindow);
    //refresh();
    //wrefresh(inputWindow);

    Input::line_func = [&](const string &line)
        {
            mainWindow << Move({0, 1}) << "<" << +Bold << line << -Bold << "> " << printWidth(line) << ClrToEol << Refresh;
        };
    Input::setup(&inputWindow);

    i = 0;
    while (1)
    {
        Input::handle();

        mainWindow << Move({0, 4}) << mainWindow.size().x << " " << mainWindow.size().y << " - " << i << ClrToEol << Refresh;

        Input::fixCursor();

        usleep(1000 * 1);
    }

    endwin();
    return 0;
}
