// TODO

#ifndef ILRD_USER_LIST_HPP
#define ILRD_USER_LIST_HPP

#include <memory> //std::unique_ptr
#include <string> // string

#include "user.hpp"

namespace ilrd {

class User;

class Command {
public:
    Command(User& user, std::string args);
    virtual ~Command() = default;

    const std::string& get_args() const;
    const User& get_user() const;
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

    static std::unique_ptr<Command> creator(User& user, std::string msg);
};

class ChangeName : public Command {
public:
    ChangeName(User& user, std::string new_name);
    ~ChangeName() = default;

    void execute() override;

    static std::unique_ptr<Command> creator(User& user, std::string new_name);
};

class List : public Command {
public:
    List(User& user);
    ~List() = default;

    void execute() override;

    static std::unique_ptr<Command> creator(User& user);
};

class Whisper : public Command {
public:
    Whisper(User& user, std::string name, std::string msg);
    ~Whisper() = default;

    void execute() override;

    static std::unique_ptr<Command>
    creator(User& user, std::string name, std::string msg);

private:
    std::string m_name;
};

} // namespace ilrd

#endif // ILRD_USER_LIST_HPP
