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

// static void check_equal(T result, T expected, int line, bool verbose = false,
//                std::string message = "");

int main()
{
    int socket_fd(12);

    std::shared_ptr<User> user(new User(Socket(socket_fd)));
    // (user->get_fd(), socket_fd + 1, __LINE__, true);

    user->set_name("name");

    CHECK_EQUAL(user->get_name() == "name", true, true, "checking equal name");

    // check_equal(user->get_name(), "name", __LINE__);
}
