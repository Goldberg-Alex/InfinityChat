//-----------------------------------------
// Written by - 		Evgeny
// File Name-           command.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the implementation of the command class and it's deriving classes.
// each type of command has it's own class with it's own execute() command.

//------------------------------------------------------------------------------
#include <algorithm> //std::for_each
#include <utility>   //std::forward

#include "command.hpp"
#include "logger.hpp"
namespace ilrd {

Command::Command(CommandParams&& params)
    : m_params(std::forward<CommandParams>(params))
{}

const std::string& Command::get_args() const
{
    return (m_params.args);
}

const std::shared_ptr<User> Command::get_user() const
{
    return (m_params.user);
}

//------------------------------------------------------------------------------

Message::Message(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

std::unique_ptr<Command> Message::create(CommandParams&& params)
{

    try {
        std::unique_ptr<Command> message(
            new Message(std::forward<CommandParams>(params)));
        return message;
    } catch (std::bad_alloc& e) {
        LOG(ERROR, "failed to create Message");
        throw;
    }
}

const std::string Message::key("/say ");

void Message::execute()
{
    LOG(DEBUG, "sending message: " + Command::m_params.args);
    std::string msg(Command::m_params.user->get_name() + ": " + Command::m_params.args);

    for (auto&& iter = Command::m_params.list.begin();
         iter != Command::m_params.list.end();
         ++iter) {

        if (iter->second->get_fd() == Command::m_params.user->get_fd()) {
            continue;
        }

        iter->second->get_socket().send(msg);
    }
}

//------------------------------------------------------------------------------
// List::List(std::shared_ptr<User> user, const UserList& list)
//     : Command(user, ""), m_list(list)
// {}

// std::unique_ptr<Command> List::create(CommandParams&& params)
// {
//     std::unique_ptr<Command> list(new List(params.user, params.list));
//     if (!list) {
//         LOG(ERROR, "failed to create List");
//         throw std::bad_alloc();
//     }

//     return list;
// }

// void List::execute()
// {
//     // std::string msg(m_user);
// }

} // namespace ilrd
