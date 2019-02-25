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

    if (argc < 2) {
        std::string str("Not enough arguments to program.\n");
        LOG(ERROR, str);
        std::cout << str << " Expected: "<< argv[0] <<" ip port" << '\n';
        return (1);
    }

    std::string ip_address(argv[1]);
    std::string port(argv[2]);

    // Alex computer: 18
    // Evgeny computer: 21
    // std::string ip_address("10.3.0.21");
    // std::string port("10000");
    Socket socket (-1);
try
{
    socket = Socket::create(ip_address, port);
}
catch(const std::exception& e)
{
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

    bool stop(false);
    while (!stop) {

        int num_events = epoll.wait(-1);
        LOG(DEBUG, "exit at epoll wait");

        for (int i = 0; i < num_events; i++) {
            if (STDIN_FILENO == epoll[i].m_fd) {

                getline(std::cin, msg);

                if (msg[0] != '/') {
                    msg = "/say " + msg;
                }

                socket.send(msg);
                LOG(DEBUG, "Message sent: " + msg);
                if (msg == "/quit") {
                    stop = true;
                    LOG(INFO, "user is out");
                    break;
                }

            } else if (socket.get_fd() == epoll[i].m_fd) {
                msg = socket.receive();
                LOG(DEBUG, "Message received: " + msg);
                if (msg.size()) {
                    std::cout << msg << '\n';
                } else {
                    stop = true;
                    LOG(INFO, "server disconnected");
                    std::cout << "server disconnected" << '\n';
                    break;
                }
            }
        }
    }

    LOG(INFO, "client closing");
    return 0;
}

//------------------------------------------------------------------------------
