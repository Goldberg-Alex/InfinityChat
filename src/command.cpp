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

Command::Command(User& user, std::string args) : m_user(user), m_args(args)
{}

const std::string& Command::get_args() const
{
    return m_args;
}

const User& Command::get_user() const
{
    return m_user;
}

//------------------------------------------------------------------------------

Message::Message(User& user, std::string msg) : Command(user, msg)
{}

std::unique_ptr<Command> Message::creator(User& user, std::string msg)
{}

void Message::execute()
{}

} // namespace ilrd
