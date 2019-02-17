// TODO

#ifndef ILRD_COMMAND_HPP
#define ILRD_COMMAND_HPP

#include <memory> //std::unique_ptr
#include <string> // string

#include "user_list.hpp"
#include "user.hpp"

namespace ilrd {

class User;

struct CommandParams {
    std::shared_ptr<User> user;
    std::string args;
    UserList& list;
};

//------------------------------------------------------------------------------
class Command {
public:
    Command(std::shared_ptr<User> user, std::string args);
    virtual ~Command() = default;

    const std::string& get_args() const;
    const std::shared_ptr<User> get_user() const;
    virtual void execute() = 0;

    Command(Command&&) = default;
    Command& operator=(Command&&) = default;

    Command(const Command&) = delete;
    Command& operator=(Command&) = delete;

protected:
    std::shared_ptr<User> m_user;
    std::string m_args;
};

//------------------------------------------------------------------------------
class Message : public Command {
public:
    ~Message() = default;

    void execute() override;

    // parameters: std::shared_ptr<User> user, std::string msg
    static std::unique_ptr<Command> creator(CommandParams&& params);

private:
    Message(std::shared_ptr<User> user, std::string msg);
};

//------------------------------------------------------------------------------
class ChangeName : public Command {
public:
    ~ChangeName() = default;

    void execute() override;

    // parameters: std::shared_ptr<User> user, std::string new_name
    static std::unique_ptr<Command> creator(CommandParams&& params);

private:
    ChangeName(std::shared_ptr<User> user, std::string new_name);
};

//------------------------------------------------------------------------------
class List : public Command {
public:
    ~List() = default;

    void execute() override;

    // parameters: std::shared_ptr<User> user, UserList& list
    static std::unique_ptr<Command> creator(CommandParams&& params);

private:
    List(std::shared_ptr<User> user, const UserList& list);

    const UserList& m_list;
};

//------------------------------------------------------------------------------
class Whisper : public Command {
public:
    ~Whisper() = default;

    void execute() override;

    // parameters: std::shared_ptr<User> user, std::string name, std::string msg
    static std::unique_ptr<Command> creator(CommandParams&& params);

private:
    Whisper(std::shared_ptr<User> user, std::string name, std::string msg);

    std::string m_name;
};

//------------------------------------------------------------------------------
} // namespace ilrd

#endif // ILRD_COMMAND_HPP
