//-----------------------------------------
// Written by - 		Evgeny
// File Name-           command.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the implementation of the command class and it's deriving classes.
// each type of command has it's own class with it's own execute() command.

//------------------------------------------------------------------------------

#include "command.hpp"
#include "logger.hpp" 

namespace ilrd {

Command::Command(std::shared_ptr<User> user, std::string args)
    : m_user(user), m_args(args)
{}

const std::string& Command::get_args() const
{
    return m_args;
}

const std::shared_ptr<User> Command::get_user() const
{
    return m_user;
}

//------------------------------------------------------------------------------

Message::Message(std::shared_ptr<User> user, std::string msg)
    : Command(user, msg)
{}

std::unique_ptr<Command> Message::create(CommandParams&& params)
{
    std::unique_ptr<Command> message(new Message(params.user, params.args));
    if (!message) {
        LOG(ERROR, "failed to create Message");
        throw std::bad_alloc();
    }

    return message;
}

void Message::execute()
{
    // std::string msg(m_user);
}

//------------------------------------------------------------------------------
List::List(std::shared_ptr<User> user, const UserList& list)
    : Command(user, ""), m_list(list)
{}

std::unique_ptr<Command> List::create(CommandParams&& params)
{
    std::unique_ptr<Command> list(new List(params.user, params.list));
    if (!list) {
        LOG(ERROR, "failed to create List");
        throw std::bad_alloc();
    }

    return list;
}

void List::execute()
{
    // std::string msg(m_user);
}

} // namespace ilrd
