#include "../include/user_list.hpp"

namespace ilrd {
UserList::UserList() : m_fd_to_user(), m_name_to_user()
{}

void UserList::insert(User&& user_)
{
    int fd(user_.get_fd());
    m_fd_to_user[fd] = std::move(user_);

    User& user = m_fd_to_user[fd];
    m_name_to_user[user.get_name()] = user;
}

} // namespace ilrd
