#include "CursesStream.hpp"

CursesStream::CursesStream(WINDOW * const new_win) : CursesStreamBuf(new_win), std::ios(0), std::ostream(static_cast<CursesStreamBuf*>(this)), win(new_win)
{

}

CursesStream::~CursesStream()
{
    sync();
}

CursesStream::operator WINDOW * const() const
{
    return win;
}


