//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include <cctype>    // std::isprint
#include <chrono>    //std::chrono::system_clock::now()
#include <ctime>     //std::localtime()
#include <deque>     //std::dqueue
#include <iomanip>   //std::put_time()
#include <iostream>  // cin
#include <ncurses.h> // tui
#include <sstream>   //std::stringstream
#include <string>    // string
#include <thread>    // std::thread
#include <unistd.h>  //STDIN_FILENO

#include "config.hpp"  //TCP_LISTEN_PORT
#include "epoll.hpp"   //Epoll
#include "logger.hpp"  //Logger
#include "ncurses.hpp" // tui RAII
#include "socket.hpp"  //Socket
//------------------------------------------------------------------------------
using namespace ilrd;

void push_msg(std::string& msg, std::deque<std::string>& message_queue)
{
    auto now = std::time(nullptr);

    std::stringstream time;
    time << "["<< (std::put_time(std::localtime(&now), "%T")) << "] ";
    msg = time.str() + msg;
    int i(0);
    
    do {
        size_t to(0);
        to = msg.find_first_of("\n");
        if (to == std::string::npos) {
            to = msg.size();
        }

        std::string sub_msg(
            (i++ ? std::string(time.str().size(), ' ') : std::string()) +
            msg.substr(0, to + 1));

        message_queue.pop_front();
        message_queue.push_back(sub_msg);
        msg.erase(0, to + 1);
    } while (msg.size());
}

void input_listener(const Socket& socket, NCurses& tui, bool& stop)
{
    std::string str;
    while (str != "/quit" && !stop) {
        str.clear();
        tui.println("MSG: ", tui.get_rows() - 1);
        str = tui.get_line("MSG: ", tui.get_rows() - 1);
        

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

    // socket.send("/help");
    NCurses tui;

    bool stop(false);
    std::thread th(
        &input_listener, std::cref(socket), std::ref(tui), std::ref(stop));

    std::deque<std::string> message_queue(tui.get_rows() - 3);
    while (!stop) {
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
                    push_msg(msg, message_queue);

                    int i(0);
                    for (auto& iter : message_queue) {
                        tui.println(iter, i);
                        i += (iter.size() / tui.get_cols()) + 1;
                    }

                    tui.println("", tui.get_rows() - 1, 5);

                } else {
                    stop = true;
                    LOG(INFO, "server disconnected");
                    tui.println("server disconnected", tui.get_rows() - 1);
                    break;
                }
            }
        }
    }

    th.join();
    LOG(INFO, "client closing");
    return 0;
}

//------------------------------------------------------------------------------
