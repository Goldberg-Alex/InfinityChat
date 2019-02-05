//-----------------------------------------
// Written by - 		Yevgeny
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

//TODO document!

//------------------------------------------------------------------------------

#include "command.hpp"

namespace ilrd {

Command::Command(User& user, std::string args): m_user(user), m_args(args)
{}

const std::string& Command::get_args() const
{
    return m_args;
}

const User& Command::get_user() const
{
    return m_user;
}


} // namespace ilrd
