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

    // Alex computer: 18
    // Evgeny computer: 21
    std::string ip_address("10.3.0.21");
    std::string port("10000");

    Socket socket(Socket::create(ip_address, port));

    LOG(INFO, "socket connected");
    socket.send("client connected");

    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(socket.get_fd(), EPOLLIN);

    LOG(INFO, "user is up");

    std::string msg;
    
    bool stop(false);
    while (!stop) {

        int num_events = epoll.wait(-1);
        LOG(DEBUG, "exit at epoll wait");
        for (int i = 0; i < num_events; i++) {
            if (STDIN_FILENO == epoll[i].m_fd) {
                getline(std::cin, msg);
                
                if (msg == "/quit") {
                    stop = true;
                    LOG(INFO, "user is out");
                    break;
                }

                if (msg[0] != '/') {
                    msg = "/say " + msg;
                }

                socket.send(msg);
                LOG(DEBUG, "Message sent: " + msg);
                
            } else if (socket.get_fd() == epoll[i].m_fd) {
                msg = socket.receive();
                LOG(DEBUG, "Message received: " + msg);
                if (msg.size()) {
                    std::cout << msg << '\n';
                }
            }
        }
    }

    LOG(INFO, "chat is down");
    return 0;
}

//------------------------------------------------------------------------------
