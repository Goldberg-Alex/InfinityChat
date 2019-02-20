// implementation of list of users.
// it's a map with two keys
// thread safe

#ifndef ILRD_USER_LIST_HPP
#define ILRD_USER_LIST_HPP

#include <map>    // map
#include <memory> // shared_ptr
#include <string> // string

#include "user.hpp"

namespace ilrd {

class UserList {
public:
    using user_ptr = std::shared_ptr<User>;
    UserList();

    void insert(user_ptr user_);
    void remove(int fd);

    std::map<const int, user_ptr>::iterator begin();
    std::map<const int, user_ptr>::iterator end();

    // returns nullptr in failure
    user_ptr find(const std::string& name);
    user_ptr find(int fd);
    size_t size() const;
    // NOTE: change name doesnt check if new name is correct
    void change_name(user_ptr user, const std::string& new_name);

    UserList(UserList&&) = default;
    UserList& operator=(UserList&&) = default;

    UserList(const UserList&) = delete;
    UserList& operator=(UserList&) = delete;

private:
    std::map<const int, user_ptr> m_fd_to_user;
    std::map<std::string, user_ptr> m_name_to_user;
};

} // namespace ilrd

#endif // ILRD_USER_LIST_HPP
