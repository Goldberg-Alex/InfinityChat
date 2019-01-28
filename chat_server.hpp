// the server part of the chat program

#ifndef ILRD_CHAT_SERVER_HPP
#define ILRD_CHAT_SERVER_HPP

#include <string> //std::string

#include "code_utils.hpp"

//------------------------------------------------------------------------------

namespace ilrd {
class User;

class Command {
public:
    Command() = default;
    Command& operator=(Command&&) = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    Command(Command&&) = default;
    ~Command() = default;

    User& get_user();
    const std::string& get_task();
    const std::string& get_args();

private:
    User& m_user;
    std::string m_task;
    std::string m_args;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_CHAT_SERVER_HPP
