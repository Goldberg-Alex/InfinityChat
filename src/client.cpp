//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include <iostream> // cin
#include <unistd.h> //STDIN_FILENO
#include <string>   // string

#include "epoll.hpp"           //Epoll
#include "logger.hpp"          //Logger
#include "socket.hpp"          //Socket
//------------------------------------------------------------------------------
using namespace ilrd;


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        // BAD, very very bad, change it!
        throw "git me ip, motherf*cker";
    }

    Socket socket(Socket::create(argv[1], argv[2]));
    
    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(socket.get_fd(), EPOLLIN);

    LOG(INFO, "user is up");

    bool stop(false);
    while(!stop) {
        break;
        int num_events = epoll.wait(0);
        for(int i = 0; i < num_events; i++) {
            if (STDIN_FILENO == epoll[i].m_fd) {
                std::string str;
                std::cin >> str;
                if (str[0] != '/') {
                    str = "/say" + str;
                }

                if (str == "/quit") {
                    stop = true;
                    LOG(INFO, "user is out");
                    break;
                }

                socket.send(str);
            } else {
                //get something from the server
            }
        }
        
    }

    return 0;
}

//------------------------------------------------------------------------------
