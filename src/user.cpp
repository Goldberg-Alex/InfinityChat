#include <sstream> // stringstream

#include "../include/user.hpp"

namespace ilrd {
User::User(int fd) : m_fd(fd), m_name(std::to_string(fd))
{}

void User::set_name(const std::string& name)
{
    m_name = name;
}

int User::get_fd() const 
{
    return m_fd;
}

const std::string& User::get_name() const
{
    return m_name;
}

} // namespace ilrd
