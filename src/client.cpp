//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include <cctype>    // std::isprint
#include <deque>     //std::dqueue
#include <iostream>  // cin
#include <ncurses.h> // gui
#include <string>    // string
#include <thread>    // std::thread
#include <unistd.h>  //STDIN_FILENO

#include "config.hpp" //TCP_LISTEN_PORT
#include "epoll.hpp"  //Epoll
#include "logger.hpp" //Logger
#include "socket.hpp" //Socket
//------------------------------------------------------------------------------
using namespace ilrd;

void get_string(std::string& str, int row)
{
    char a('0');
    noecho();
    while (a != '\n') {
        clrtoeol();
        mvprintw(row - 1, 0, "MSG: ");

        a = getch();
        if (std::isprint(a)) {
            str += a;
        } else if (a == 0x7f && str.size()) {
            str.pop_back();
        }

        mvprintw(row - 1, 5, str.c_str());
        refresh();
    }
}

void input_listener(const Socket& socket, int max_rows, bool& stop)
{
    std::string str;
    while (str != "/quit" && !stop) {
        str.clear();
        get_string(str, max_rows - 1);

        if (str[0] != '/') {
            str = "/say " + str;
        }

        socket.send(str);
        LOG(DEBUG, "Message sent: " + str);
    }
    stop = true;
}

int main(int argc, char const* argv[])
{
    // this is just to init the logger to the correct file
    Handleton<Logger>::get_instance("client_log.txt");

    if (argc < 2) {
        std::string str("Not enough arguments to program.\n");
        LOG(ERROR, str);
        std::cout << str << " Expected: " << argv[0] << " ip port" << '\n';
        return (1);
    }

    std::string ip_address(argv[1]);

    // if no port given, use the default

    std::string port(TCP_LISTEN_PORT);
    if (argc > 2) {
        port = argv[2];
    }

    // Alex computer: 18
    // Evgeny computer: 21
    // std::string ip_address("10.3.0.21");
    // std::string port("10000");
    Socket socket(-1);
    try {
        socket = Socket::create(ip_address, port);
    } catch (const std::exception& e) {
        std::cout << "Error connecting to server." << '\n';

        return (1);
    }

    LOG(INFO, "socket connected");

    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(socket.get_fd(), EPOLLIN);

    LOG(INFO, "client is ready");

    std::string msg;

    socket.send("/help");
    size_t max_rows, max_cols;

    initscr();
    getmaxyx(stdscr, max_rows, max_cols);

    bool stop(false);
    std::thread th(
        &input_listener, std::cref(socket), max_rows, std::ref(stop));

    while (!stop) {

        std::deque<std::string> message_queue(max_rows - 3);

        int num_events = epoll.wait(-1);
        LOG(DEBUG, "exit at epoll wait");

        for (int i = 0; i < num_events; i++) {
            if (socket.get_fd() == epoll[i].m_fd) {
                msg = socket.receive();
                LOG(DEBUG, "Message received: " + msg);

                if (msg.size()) {
                    // if (message_queue.size() == max_rows - 3) {
                    //     message_queue.pop_front();
                    // }

                    message_queue.push_back(msg);

                    int i(0);
                    for (auto& iter : message_queue) {
                        move(i, 0);
                        clrtoeol();
                        printw(iter.c_str());
                        i += (iter.size() / max_cols) + 1;
                    }

                    refresh();
                } else {
                    stop = true;
                    LOG(INFO, "server disconnected");
                    std::cout << "server disconnected" << '\n';
                    break;
                }
            }
        }
    }

    th.join();
    endwin();
    LOG(INFO, "client closing");
    return 0;
}

//------------------------------------------------------------------------------
