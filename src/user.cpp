//-----------------------------------------
// Written by - 		Evgeny
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// TODO document

#include "user.hpp"

namespace ilrd {
User::User(Socket&& sock)
    : m_socket(std::move(sock)), m_name(std::to_string(m_socket.get_fd()))
{}

void User::set_name(const std::string& name)
{
    m_name = name;
}

int User::get_fd() const
{
    return (m_socket.get_fd());
}

const std::string& User::get_name() const
{
    return (m_name);
}
const Socket& User::get_socket()
{
    return (m_socket);
}
} // namespace ilrd
