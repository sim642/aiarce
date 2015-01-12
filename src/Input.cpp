#include "Input.hpp"
#include "CursesManip.hpp"
#include "StringUtil.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

std::function<void(const std::string&)> Input::line_func;
fd_set Input::fds;
Window *Input::window = nullptr;
int Input::view_point = 0;
int Input::cur_point = 0;

void Input::setup(Window * const new_window)
{
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Input::window = new_window;
    //rl_callback_handler_install(">", Input::line_hook);
    rl_callback_handler_install("", Input::line_hook);
    rl_input_available_hook = Input::input_available;
    rl_redisplay_function = Input::redisplay_hook;

    rl_forced_update_display();
}

void Input::handle()
{
    if (Input::input_available())
        rl_callback_read_char();
}

void Input::fixCursor()
{
    Vec size = Input::window->size();
    *Input::window << Move({cur_point % size.x, cur_point / size.x}) << Refresh;
}

int Input::input_available()
{
    timeval tv = {0, 0};
    FD_ZERO(&Input::fds);
    FD_SET(0, &Input::fds);
    return select(1, &Input::fds, NULL, NULL, &tv);
}

void Input::redisplay_hook()
{
    int pos = to_wide(std::string(rl_line_buffer, rl_point)).size();
    int len = Input::window->length();
    view_point = std::max(0, (pos - len % 2) / (len / 2) - 1) * (len / 2);
    cur_point = pos - view_point;

    *Input::window << Move({0, 0}) << from_wide(to_wide(rl_line_buffer).substr(view_point, len)) << ClrToBot << Refresh;

    /*mvwprintw(*Input::window, 0, 0, "%s%s%d", rl_display_prompt, rl_line_buffer, wstr.size());
    wclrtobot(*Input::window);
    wrefresh(*Input::window);*/

    /*move(15, 0);
    for (char* it = rl_line_buffer; *it; it++)
        printw("%x ", *it);
    clrtobot();
    refresh();*/
}

void Input::line_hook(char *line)
{
    add_history(line);
    Input::line_func(std::string(line));
    std::free(line);
}
