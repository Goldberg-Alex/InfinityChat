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
    std::shared_ptr<User> user(new User(Socket(socket_fd)));
    check_equal(user->get_fd(), socket_fd, __LINE__);

    user->set_name("name");
    check_equal(user->get_name(), std::string("name"), __LINE__);

    // Checking UserList
    UserList list;

    // Insert user, compare find(fd) and find(user_name)
    // compare find and user ptr
    list.insert(user);
    check_equal(list.find(socket_fd), list.find("name"), __LINE__);
    check_equal(list.find(socket_fd), user, __LINE__);

    // check iterators
    check_equal(++list.begin() == list.end(), true, __LINE__);
    list.remove(socket_fd);
    check_equal(list.begin() == list.end(), true, __LINE__);

    // change name, search for it
    list.change_name(user, std::string("new_name"));
    check_equal(user->get_name(), std::string("new_name"), __LINE__);
    check_equal(list.find("new_name"), user, __LINE__);

    
}
