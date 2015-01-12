#ifndef CURSESSTREAM_H
#define CURSESSTREAM_H

#include "CursesStreamBuf.hpp"
#include <ostream>
#include <ncursesw/ncurses.h>
#include "Vec.hpp"
#include <utility>

class CursesStream : /*virtual*/ private CursesStreamBuf, public std::ostream
{
public:
    CursesStream(WINDOW * const win);
    virtual ~CursesStream();

    template<typename T>
    CursesStream& operator<< (T&& val)
    {
        return static_cast<CursesStream&>(*static_cast<std::ostream*>(this) << std::forward<T>(val));
    }

    operator WINDOW * const() const;
private:
    WINDOW * const win;
};

template<>
inline CursesStream& CursesStream::operator<< (CursesStream& (&func)(CursesStream&))
{
    return func(*this);
}


#endif // CURSESSTREAM_H
