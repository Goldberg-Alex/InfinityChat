//-----------------------------------------
// Written by - 		Evgeny
// File Name-           command.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// TODO document!

//------------------------------------------------------------------------------

#include <new> // std::bad_alloc

#include "logger.hpp"
#include "command.hpp"

namespace ilrd {

Command::Command(std::shared_ptr<User> user, std::string args) : m_user(user), m_args(args)
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

Message::Message(std::shared_ptr<User> user, std::string msg) : Command(user, msg)
{}

std::unique_ptr<Command> Message::creator(std::shared_ptr<User> user, std::string msg)
{
    std::unique_ptr<Command> message(new Message(user, msg));
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

} // namespace ilrd
