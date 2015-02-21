#include "Input.hpp"
#include "CursesManip.hpp"
#include "StringUtil.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <vector>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

std::function<void(const std::string&)> Input::line_func;
fd_set Input::fds;
Window *Input::window = nullptr;
int Input::view_point = 0;
int Input::cur_point = 0;

int printWidth(const std::string &line)
{
    WINDOW *off = newwin(0, 0, 0, 0);
    wprintw(off, "%s", line.c_str());

    int y, x, my, mx;
    getyx(off, y, x);
    getmaxyx(off, my, mx);
    int ret = x + y * mx;
    delwin(off);
    return ret;
}

void Input::setup(Window * const new_window)
{
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Input::window = new_window;
    //rl_callback_handler_install(">", Input::line_hook);
    rl_callback_handler_install("->", Input::line_hook);
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
    std::string bytes(rl_line_buffer);
    std::wstring chars;

    std::vector<int> byte2char;
    std::vector<int> char2byte;
    std::vector<int> char2width;
    std::vector<int> width2char;

    std::mbstate_t state = std::mbstate_t();
    int n = 0;
    int cnt;
    wchar_t wc;
    while ((cnt = mbrtowc(&wc, bytes.c_str() + n, bytes.size() - n, &state)) > 0)
    {
        chars.push_back(wc);

        for (int i = 0; i < cnt; i++)
            byte2char.push_back(chars.size() - 1);

        char2byte.push_back(n);

        char2width.push_back(width2char.size());
        int width = printWidth(bytes.substr(n, cnt));
        for (int i = 0; i < width; i++)
            width2char.push_back(chars.size() - 1);

        n += cnt;
    }

    move(15, 0);
    printw("bytes: ");
    for (char &byte : bytes)
        printw("%x ", byte & 0xFF);
    printw("\nchars: ");
    for (wchar_t &ch : chars)
        printw("%x ", ch);
    printw("\nb2c: ");
    for (int &b2c : byte2char)
        printw("%d ", b2c);
    printw("\nc2b: ");
    for (int &c2b : char2byte)
        printw("%d ", c2b);
    printw("\nc2w: ");
    for (int &c2w : char2width)
        printw("%d ", c2w);
    printw("\nw2c: ");
    for (int &w2c : width2char)
        printw("%d ", w2c);
    clrtobot();
    refresh();

    std::wstring prompt = to_wide(rl_display_prompt);

    //int pos = to_wide(std::string(rl_line_buffer, rl_point)).size();
    int pos = printWidth(std::string(rl_line_buffer, rl_point));
    int len = std::max<int>(0, Input::window->length() - prompt.length());
    view_point = len == 0 ? 0 : std::max(0, (pos - len % 2) / (len / 2) - 1) * (len / 2);
    cur_point = pos - view_point + prompt.length();

    *Input::window << Move({0, 0}) << from_wide(prompt) << from_wide(to_wide(rl_line_buffer).substr(view_point, len)) << ClrToBot << Refresh;

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
