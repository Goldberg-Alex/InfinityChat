#ifndef ILRD_COMMAND_HPP
#define ILRD_COMMAND_HPP

#include <memory> //std::unique_ptr
#include <string> // string
#include <vector>

#include "user.hpp"
#include "user_list.hpp"

namespace ilrd {

class User;

struct CommandParams {
    std::string args;
    std::shared_ptr<User> user;
    UserList& list;
};
//------------------------------------------------------------------------------
class Command {
public:
    Command(CommandParams&& m_params);
    virtual ~Command() = default;

    const std::string& get_args() const;
    const std::shared_ptr<User> get_user() const;
    virtual void execute() = 0;

    Command(Command&&) = default;
    Command& operator=(Command&&) = default;

    Command(const Command&) = delete;
    Command& operator=(Command&) = delete;

    static std::vector<std::string> s_command_list;
    static void init_command_list();

protected:
    CommandParams m_params;

private:
    static size_t push_get_index(std::string&& str);
};

//------------------------------------------------------------------------------
class Message : public Command {
public:
    ~Message() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    Message(CommandParams&& params);

    void single_message(const Socket& socket);
};

//------------------------------------------------------------------------------
class ChangeName : public Command {
public:
    ~ChangeName() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    ChangeName(CommandParams&& params);
};

//------------------------------------------------------------------------------
class List : public Command {
public:
    ~List() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    List(CommandParams&& params);
};

//------------------------------------------------------------------------------
class Whisper : public Command {
public:
    ~Whisper() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    Whisper(CommandParams&& params);

    std::string m_name;
};

//------------------------------------------------------------------------------
class Help : public Command {
public:
    ~Help() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    Help(CommandParams&& params);
};

//------------------------------------------------------------------------------
class Quit : public Command {
public:
    ~Quit() override = default;

    void execute() override;

    static std::unique_ptr<Command> create(CommandParams&& params);
    static size_t key;

private:
    Quit(CommandParams&& params);
};

} // namespace ilrd

#endif // ILRD_COMMAND_HPP
