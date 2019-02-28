#include <thread>

#include "ncurses.hpp"

using namespace ilrd;

void foo(NCurses& tui)
{
    std::string str;

    while (str != "q") {
        str.clear();
        tui.println("MSG: ", tui.get_rows() - 1);
        str = tui.get_line("MSG: ", tui.get_rows() - 1);
        tui.println(str, 1, 10);
    }
}

int main()
{
    // uncomment to see the output
    /*
    NCurses tui;
    tui.println("Hello world!");
    
    std::thread th(&foo, std::ref(tui));

    th.join();
*/
    return 0;
}
