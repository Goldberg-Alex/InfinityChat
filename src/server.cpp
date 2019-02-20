//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			server.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the main server implementation
#include <iostream> //std::cout
#include <memory>   //std::make_shared
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
    // this is just to init the logger to the correct file
    Handleton<Logger>::get_instance("server_log.txt");

    SocketListener listener(TCP_LISTEN_PORT);
    UserList user_list;
    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(listener.get_fd(), EPOLLIN);

    Factory<Command, std::string, CommandParams> factory;

    // factory.add(Message::key, &Message::create);
    // add the rest of the tasks in the same way

    bool stop = false;

    LOG(INFO, "entering event loop.");

    while (!stop) {

        int num_events = epoll.wait(-1);
        for (int i = 0; i < num_events; i++) {

            if (STDIN_FILENO == epoll[i].m_fd) {
                // handle stdin
            } else if (listener.get_fd() == epoll[i].m_fd) {
                LOG(INFO, "new user connected");
                Socket socket = listener.connect();
                auto user = std::make_shared<User>(std::move(socket));
                user->get_socket().send("connected");
                user_list.insert(user);
                LOG(INFO, "created new user and inserted into user list");

            } else if (epoll[i].m_event_type == EPOLLIN) {
                auto user = user_list.find(epoll[i].m_fd);
                const Socket& socket = user->get_socket();

                LOG(INFO, "received message");
                auto message = socket.receive();
                socket.send(message);

                std::cout << "message:" << '\n';
                std::cout << message << '\n';

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
