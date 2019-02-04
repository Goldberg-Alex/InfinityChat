#include "user_list.hpp"

namespace ilrd {
UserList::UserList() : m_fd_to_user(), m_name_to_user()
{}

void UserList::insert(UserList::user_ptr user)
{
    int fd(user->get_fd());
    m_fd_to_user.insert(std::pair<int, UserList::user_ptr >(fd, user));

    m_name_to_user.insert(std::pair<std::string, UserList::user_ptr >(user->get_name(), user));
}

void UserList::remove(int fd)
{
    auto user = m_fd_to_user[fd];
    m_fd_to_user.erase(fd);
    m_name_to_user.erase(user->get_name());
}

std::map<const int, UserList::user_ptr >::iterator UserList::begin()
{
    return m_fd_to_user.begin();
}

std::map<const int, UserList::user_ptr >::iterator UserList::end()
{
    return m_fd_to_user.end();
}

UserList::user_ptr UserList::find(const std::string& name)
{
    auto iter = m_name_to_user.find(name);
    if (iter == m_name_to_user.end()) {
        return user_ptr(nullptr);
    }

    return iter->second;
}

UserList::user_ptr UserList::find(int fd)
{
    auto iter = m_fd_to_user.find(fd);
    if (iter == m_fd_to_user.end()) {
        return user_ptr(nullptr);
    }

    return iter->second;
}

} // namespace ilrd
