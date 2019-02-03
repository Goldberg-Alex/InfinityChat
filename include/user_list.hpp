// implementation of list of users.
// it's a map with two keys
// thread safe

#ifndef ILRD_USER_LIST_HPP
#define ILRD_USER_LIST_HPP

#include <map>    // map
#include <string> // string

#include "user.hpp"

namespace ilrd {
class UserList {
    explicit UserList();

    void insert(User&& user_);
    void remove(int fd);

    std::map<const int, User>::iterator begin();
    std::map<const int, User>::iterator end();

    User& find(const std::string& name);
    User& find(int fd);

    void change_name(User& user, const std::string& new_name);

    UserList(UserList&&) = default;
    UserList& operator=(UserList&&) = default;

    UserList(const UserList&) = delete;
    UserList& operator=(UserList&) = delete;

private:
    std::map<const int, User> m_fd_to_user;
    std::map<std::string, User&> m_name_to_user;
};

} // namespace ilrd

#endif // ILRD_USER_LIST_HPP
