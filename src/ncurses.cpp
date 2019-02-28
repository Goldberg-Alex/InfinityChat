#include <ncurses.h> // tui
#include <cctype>    // std::isprint

#include "ncurses.hpp"

namespace ilrd {

NCurses::NCurses()
{
    initscr();
    getmaxyx(stdscr, m_max_rows, m_max_cols);
}

NCurses::~NCurses()
{
    endwin();
}

int NCurses::get_rows() const 
{
    return m_max_rows;
}

int NCurses::get_cols() const
{
    return m_max_cols;
}

void NCurses::println(const std::string& str, int row, int col)
{
    std::lock_guard<std::mutex> lock(m_lock);
    move(row, col);
    clrtoeol();
    printw(str.c_str());
    refresh();
}

std::string NCurses::get_line(const std::string& msg, int row, int col)
{
    char c('0');
    std::string str;
    noecho();

    while (c != '\n') {
        c = getch();
        if (std::isprint(c)) {
            str += c;
        } else if (c == 0x7f && str.size()) {
            str.pop_back();
        }

        println(msg + str, row, col);
    }

    println("", row, col);
    return str;
}

std::string NCurses::get_line(int row, int col)
{
    return get_line("", row, col);
}

} // namespace ilrd
