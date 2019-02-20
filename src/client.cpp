//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include <iostream> // cin
#include <string>   // string
#include <unistd.h> //STDIN_FILENO

#include "epoll.hpp"  //Epoll
#include "logger.hpp" //Logger
#include "socket.hpp" //Socket
//------------------------------------------------------------------------------
using namespace ilrd;

int main(int argc, char const* argv[])
{
    // this is just to init the logger to the correct file
    Handleton<Logger>::get_instance("client_log.txt");

    // if (argc < 2) {
    //     std::string str("not enough arguments to program");
    //     LOG(ERROR, str);
    //     throw std::runtime_error(str);
    // }

    // std::string ip_address(argv[1]);
    // std::string port(argv[2]);

    std::string ip_address("10.3.0.18");
    std::string port("10000");

    Socket socket(Socket::create(ip_address, port));

    LOG(INFO, "socket connected");

    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(socket.get_fd(), EPOLLIN);

    LOG(INFO, "user is up");

    bool stop(false);
    while (!stop) {

        int num_events = epoll.wait(-1);
        for (int i = 0; i < num_events; i++) {
            if (STDIN_FILENO == epoll[i].m_fd) {
                std::string str;
                std::cin >> str;
                if (str[0] != '/') {
                    str = "/say " + str;
                    socket.send(str);
                }

                if (str == "/quit") {
                    stop = true;
                    LOG(INFO, "user is out");
                }
            } else if (socket.get_fd() == epoll[i].m_fd) {
                auto msg = socket.receive();
                if (msg.size()) {
                    std::cout << "message: " << msg << '\n';
                }
            }
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
