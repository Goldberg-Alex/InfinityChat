// TODO

#ifndef ILRD_USER_LIST_HPP
#define ILRD_USER_LIST_HPP

#include <string> // string

#include "user.hpp"

namespace ilrd {
class User;
class Command {
public:
    Command(User& user, std::string args);
    virtual ~Command();

    const std::string& get_args();
    const User& get_user();
    virtual void execute() = 0;

    Command(Command&&) = default;
    Command& operator=(Command&&) = default;

    Command(const Command&) = delete;
    Command& operator=(Command&) = delete;
protected:
    User& m_user;
    std::string m_args;
};

class Message : public Command {
public:
    Message(User& user, std::string msg);
    ~Message() = default;

    void execute() override;
};

class ChangeName : public Command {
public:
    ChangeName(User& user, std::string new_name);
    ~ChangeName() = default;

    void execute() override;
};

class List : public Command {
public:
    List(User& user);
    ~List() = default;

    void execute() override;
};

class Whisper : public Command {
public:
    Whisper(User& user, std::string name, std::string msg);
    ~Whisper() = default;

    void execute() override;

private:
    std::string m_name;
};

} // namespace ilrd

#endif // ILRD_USER_LIST_HPP
