//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			server.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the main server implementation
#include <unistd.h> //STDIN_FILENO

#include "command.hpp"         //commands
#include "config.hpp"          //TCP_LISTEN_PORT
#include "epoll.hpp"           //Epoll
#include "factory.hpp"         //Factory
#include "logger.hpp"          //Logger
#include "socket_listener.hpp" //SocketCreator

//------------------------------------------------------------------------------
using namespace ilrd;

//------------------------------------------------------------------------------
void init_server()
{}

//------------------------------------------------------------------------------

int main()
{
    SocketListener listener(TCP_LISTEN_PORT);

    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(listener.get_fd(), EPOLLIN);

    Factory<Command, std::string, CommandParams> factory;

    // factory.add(Message::key, &Message::create);
    // add the rest of the tasks in the same way

    bool stop = false;

    LOG(INFO, "entering event loop.");

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
