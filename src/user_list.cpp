//-----------------------------------------
// Written by - 		Evgeny
// File Name-           user_list.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// Implementation for Two-Keys Map

#include "user_list.hpp"
#include "logger.hpp"

namespace ilrd {
UserList::UserList() : m_fd_to_user(), m_name_to_user()
{
    LOG(INFO, "created user list");
}

void UserList::insert(user_ptr user)
{

    m_fd_to_user.insert(std::pair<int, user_ptr>(user->get_fd(), user));

    m_name_to_user.insert(
        std::pair<std::string, user_ptr>(user->get_name(), user));

    LOG(DEBUG, "inserted new user to list");
}

void UserList::remove(int fd)
{
    auto user = m_fd_to_user[fd];
    m_fd_to_user.erase(fd);
    m_name_to_user.erase(user->get_name());

    LOG(DEBUG, "user removed from list");
}

std::map<const int, UserList::user_ptr>::iterator UserList::begin()
{
    return m_fd_to_user.begin();
}

std::map<const int, UserList::user_ptr>::iterator UserList::end()
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
        LOG(ERROR, "can't find user via fd");
        return user_ptr(nullptr);
    }

    return iter->second;
}

void UserList::change_name(user_ptr user, const std::string& new_name)
{
    std::string old_name(user->get_name());
    m_name_to_user.erase(old_name);

    user->set_name(new_name);
    m_name_to_user.insert(
        std::pair<std::string, user_ptr>(user->get_name(), user));

    LOG(DEBUG, "changed name for user");
}

} // namespace ilrd
