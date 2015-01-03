#ifndef INPUT_H
#define INPUT_H

#include <boost/noncopyable.hpp>
#include "Window.hpp"
#include <functional>
#include <string>
#include <sys/types.h>

class Input : public boost::noncopyable
{
public:
    static void setup(Window * const new_window);
    static void handle();
    static void fixCursor();
    static int input_available();

    static std::function<void(const std::string&)> line_func;
private:
    static void redisplay_hook();
    static void line_hook(char *line);

    static Window *window;
    static fd_set fds;

    static int view_point, cur_point;
};

#endif // INPUT_H
