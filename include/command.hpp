// TODO

#ifndef ILRD_COMMAND_HPP
#define ILRD_COMMAND_HPP

#include <memory> //std::unique_ptr
#include <string> // string

#include "user.hpp"
#include "user_list.hpp"

namespace ilrd {

class User;

struct CommandParams {
    std::string args;
    std::shared_ptr<User> user;
    const UserList& list;
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
    ~Message() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static constexpr char key[] = "say";

private:
    Message(std::shared_ptr<User> user, std::string msg);
};

//------------------------------------------------------------------------------
class ChangeName : public Command {
public:
    ~ChangeName() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);

private:
    ChangeName(std::shared_ptr<User> user, std::string new_name);
};

//------------------------------------------------------------------------------
class List : public Command {
public:
    ~List() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);

private:
    List(std::shared_ptr<User> user, const UserList& list);
    const UserList& m_list;
};

//------------------------------------------------------------------------------
class Whisper : public Command {
public:
    ~Whisper() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);

private:
    Whisper(std::shared_ptr<User> user, std::string name, std::string msg);

    std::string m_name;
};

//------------------------------------------------------------------------------
} // namespace ilrd

#endif // ILRD_COMMAND_HPP
