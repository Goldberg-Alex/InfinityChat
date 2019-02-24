//-----------------------------------------
// Written by - 		Evgeny
// File Name-           user_list.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// Implementation for Two-Keys Map
#include <stdexcept> // std::logic_error

#include "user_list.hpp"
#include "logger.hpp"

namespace ilrd {
UserList::UserList() : m_fd_to_user(), m_name_to_user()
{
    LOG(INFO, "created user list");
}

void UserList::insert(user_ptr user)
{
    ScopeLock<std::recursive_mutex> lock(m_lock);
    m_fd_to_user.insert(std::pair<int, user_ptr>(user->get_fd(), user));

    try {
        m_name_to_user.insert(
            std::pair<std::string, user_ptr>(user->get_name(), user));
    } catch (const std::exception& e) {
        m_fd_to_user.erase(user->get_fd());
        LOG(ERROR, "failed to insert user into user list");
        throw;
    }

    LOG(DEBUG, "inserted new user to list");
}

void UserList::remove(int fd)
{
    ScopeLock<std::recursive_mutex> lock(m_lock);

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
    ScopeLock<std::recursive_mutex> lock(m_lock);

    auto iter = m_name_to_user.find(name);
    if (iter == m_name_to_user.end()) {
        return user_ptr(nullptr);
    }

    return iter->second;
}

UserList::user_ptr UserList::find(int fd)
{
    ScopeLock<std::recursive_mutex> lock(m_lock);

    auto iter = m_fd_to_user.find(fd);
    if (iter == m_fd_to_user.end()) {
        std::string error("can't find user via fd");
        LOG(ERROR, error);
        throw std::logic_error(error);
    }

    return iter->second;
}

size_t UserList::size()
{
    ScopeLock<std::recursive_mutex> lock(m_lock);
    return (m_fd_to_user.size());
}

void UserList::change_name(user_ptr user, const std::string& new_name)
{
    ScopeLock<std::recursive_mutex> lock(m_lock);

    if (find(new_name)) {
        // new_name is taken, return
        return;
    }

    std::string old_name(user->get_name());
    
    try {
        m_name_to_user.insert(
            std::pair<std::string, user_ptr>(new_name, user));
    } catch (const std::exception& e) {
        LOG(ERROR, "failed to change name");
        throw;
    }

    user->set_name(new_name);
    m_name_to_user.erase(old_name);

    LOG(DEBUG, "changed name for user");
}

} // namespace ilrd
