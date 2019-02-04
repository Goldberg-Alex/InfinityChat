#include "user_list.hpp"

namespace ilrd {
UserList::UserList() : m_fd_to_user(), m_name_to_user()
{}

void UserList::insert(std::shared_ptr<User> user)
{
    int fd(user->get_fd());
    m_fd_to_user.insert(std::pair<int, std::shared_ptr<User> >(fd, user));

    m_name_to_user.insert(std::pair<std::string, std::shared_ptr<User> >(user->get_name(), user));
}

} // namespace ilrd
