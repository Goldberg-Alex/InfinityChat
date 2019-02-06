//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			server.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the main server implementation
#include <unistd.h> //STDIN_FILENO

#include "config.hpp"          //TCP_LISTEN_PORT
#include "epoll.hpp"           //Epoll
#include "socket_listener.hpp" //SocketCreator

//------------------------------------------------------------------------------
using namespace ilrd;

//------------------------------------------------------------------------------
void init_server();
//------------------------------------------------------------------------------

int main()
{
    Epoll epoll;
    SocketListener listener(TCP_LISTEN_PORT);

    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(listener.get_fd(), EPOLLIN);

    bool stop = false;

    while (!stop) {
        int num_events = epoll.wait(0);
        for (int i = 0; i < num_events; i++) {

            if (STDIN_FILENO == epoll[i].m_fd) {
                // handle stdin
            } else if (listener.get_fd() == epoll[i].m_fd) {
                // create new user
                // insert user into the user list
            } else if (epoll[i].m_event_type == EPOLLIN) {
                // create command
                // insert command into the command queue
            } else if (epoll[i].m_event_type == EPOLLHUP) {
                // remove user from user list
                // delete user
            }
        }
    }

    return (0);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
