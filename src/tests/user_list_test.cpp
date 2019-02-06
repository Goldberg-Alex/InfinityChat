//-----------------------------------------
// Written by - 		Evgeny
// File Name-           user_list_test.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// Test for User class and UserList class

#include "code_utils.hpp"
#include "user_list.hpp"

using namespace ilrd;

int main()
{
    int socket_fd(12);

    // Checking User: create user, set and get name, get fd
    // ERROR on std::move in user ctor
    std::shared_ptr<User> user(new User(Socket(socket_fd)));
    CHECK_EQUAL(user->get_fd(), socket_fd, false, "1");

    user->set_name("name");
    CHECK_EQUAL(user->get_name(), std::string("name"), false, "1");

    // Checking UserList
    UserList list;

    // Insert user, compare find(fd) and find(user_name)
    // compare find and user ptr
    list.insert(user);
    CHECK_EQUAL(list.find(socket_fd), list.find("name"), false, "1");
    CHECK_EQUAL(list.find(socket_fd), user, false, "1");

    // check iterators
    CHECK_EQUAL(++list.begin() == list.end(), true, false, "1");
    list.remove(socket_fd);
    CHECK_EQUAL(list.begin() == list.end(), true, false, "1");

    // change name, search for it
    list.change_name(user, std::string("new_name"));
    CHECK_EQUAL(user->get_name(), std::string("new_name"), false, "1");
    CHECK_EQUAL(list.find("new_name"), user, false, "1");

    size_t count = 1001;
    for (size_t i = 100; i < count; ++i) {
        list.insert(std::shared_ptr<User>(new User(Socket(i))));
        list.change_name(list.find(i), std::to_string(i * 2));
    }

    for (size_t i = 100; i < count; i++) {
        check_equal(list.find(i)->get_name(),
                    std::to_string(i * 2),
                    __LINE__,
                    __FILE__,
                    false,
                    "1");
    }
}
