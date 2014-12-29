#include "Input.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <clocale>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

std::function<void(const std::string&)> Input::line_func;
//std::function<void()> Input::redisplay_func;
fd_set Input::fds;
Window *Input::window = nullptr;

void Input::setup(Window * const new_window)
{
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Input::window = new_window;
    rl_callback_handler_install(">", Input::line_hook);
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
    std::mbstate_t state = std::mbstate_t();
    char ch = rl_line_buffer[rl_point];
    rl_line_buffer[rl_point] = '\0';
    int pos = 1 + std::mbsrtowcs(NULL, const_cast<const char**>(&rl_line_buffer), 0, &state);
    rl_line_buffer[rl_point] = ch;
    wmove(*Input::window, 0, /*strlen(rl_display_prompt) + rl_point*/ pos);
    wrefresh(*Input::window);
}

int Input::input_available()
{
    /*static*/ timeval tv = {0, 0};
    FD_ZERO(&Input::fds);
    FD_SET(0, &Input::fds);
    return select(1, &Input::fds, NULL, NULL, &tv);
}

void Input::redisplay_hook()
{
    mvwprintw(*Input::window, 0, 0, "%s%s", rl_display_prompt, rl_line_buffer);
    wclrtobot(*Input::window);
    wrefresh(*Input::window);

    //Input::redisplay_func();

    move(15, 0);
    for (char* it = rl_line_buffer; *it; it++)
        printw("%x ", *it);
    clrtobot();
    refresh();
}

void Input::line_hook(char *line)
{
    add_history(line);
    Input::line_func(std::string(line));
    std::free(line);
}

/*Input& Input::get()
{
    static Input instance;
    return instance;
}*/
