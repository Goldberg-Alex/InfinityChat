//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			server.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the main server implementation
#include <cctype>   //std::isspace
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
void add_user(Epoll& epoll, UserList& user_list, SocketListener& listener)
{
    LOG(INFO, "new user connected");
    Socket socket = listener.connect();
    auto user = std::make_shared<User>(std::move(socket));
    std::cout << user->get_socket().get_fd() << std::endl;
    user->get_socket().send("server connected");
    epoll.add(user->get_socket().get_fd(), EPOLLIN);
    user_list.insert(user);

    LOG(INFO, "created new user and inserted into user list");
}

static size_t find_white_space(const std::string& str)
{
    size_t i(0);
    for (; i < str.size() && !std::isspace(str[i]); ++i) {
    }

    return i;
}

int main()
{
    // this is just to init the logger to the correct file
    Handleton<Logger>::get_instance("server_log.txt");

    SocketListener listener(TCP_LISTEN_PORT);
    UserList user_list;
    Epoll epoll;
    epoll.add(STDIN_FILENO, EPOLLIN);
    epoll.add(listener.get_fd(), EPOLLIN);

    Command::init_command_list();

    Factory<Command, std::string, CommandParams> factory;

    factory.add(Command::s_command_list[Message::key], &Message::create);
    factory.add(Command::s_command_list[ChangeName::key], &ChangeName::create);
    factory.add(Command::s_command_list[List::key], &List::create);
    factory.add(Command::s_command_list[Whisper::key], &Whisper::create);
    factory.add(Command::s_command_list[Help::key], &Help::create);
    factory.add(Command::s_command_list[Quit::key], &Quit::create);

    // add the rest of the tasks in the same way

    bool stop = false;

    LOG(INFO, "entering event loop.");

    while (!stop) {

        int num_events = epoll.wait(-1);
        LOG(DEBUG, "exit at epoll wait");
        for (int i = 0; i < num_events; i++) {

            if (STDIN_FILENO == epoll[i].m_fd) {
                stop = true;
                break;
            } else if (listener.get_fd() == epoll[i].m_fd) {
                add_user(epoll, user_list, listener);
            } else if (epoll[i].m_event_type == EPOLLIN) {
                auto user = user_list.find(epoll[i].m_fd);
                const Socket& socket = user->get_socket();

                auto message = socket.receive();

                // client disconnected
                if (message == "\0") {
                    // remove user from list
                    continue;
                }

                LOG(DEBUG, "received message: " + message);

                // parse string
                size_t end_of_command(find_white_space(message));
                std::string key = message.substr(0, end_of_command);
                std::string args;
                if (end_of_command < message.size()) {
                    args = message.substr(end_of_command + 1);
                }

                CommandParams params{args, user, user_list};

                auto command = factory.create(key, std::move(params));
                command->execute();
                // create command
                // insert command into the command queue
            } else if (epoll[i].m_event_type == EPOLLHUP) {
                LOG(INFO, "client disconnected");
            }
        }
    }

    return (0);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
