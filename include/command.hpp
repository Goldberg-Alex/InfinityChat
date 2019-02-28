#ifndef ILRD_COMMAND_HPP
#define ILRD_COMMAND_HPP

#include <memory> //std::unique_ptr
#include <string> // string
#include <vector>

#include "epoll.hpp"
#include "logger.hpp" //Logger
#include "user.hpp"
#include "user_list.hpp"

namespace ilrd {

class User;

struct CommandParams {
    std::string args;
    std::shared_ptr<User> user;
    UserList& list;
    Epoll& epoll;
};
//------------------------------------------------------------------------------
class Command {
public:
    Command(CommandParams&& m_params);
    virtual ~Command() = default;

    const std::string& get_args() const;
    const std::shared_ptr<User> get_user() const;
    virtual void execute() = 0;
    void send_global_msg(const std::string& msg);
    Command(Command&&) = default;
    Command& operator=(Command&&) = default;

    Command(const Command&) = delete;
    Command& operator=(Command&) = delete;

    static std::vector<std::string> s_command_list;
    static void init_command_list();

    template <typename T>
    static std::unique_ptr<Command> create(CommandParams&& params);

protected:
    CommandParams m_params;

private:
    static size_t push_get_index(std::string&& str);
};

template <typename T>
std::unique_ptr<Command> Command::create(CommandParams&& params)
{
    try {
        std::unique_ptr<Command> command(
            new T(std::forward<CommandParams>(params)));
        return command;
    } catch (std::bad_alloc& e) {
        LOG(ERROR, "failed to create command");
        throw;
    }
}

//------------------------------------------------------------------------------
class Message : public Command {
public:
    ~Message() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    Message(CommandParams&& params);

    void single_message(const Socket& socket);
};

//------------------------------------------------------------------------------
class ChangeName : public Command {
public:
    ~ChangeName() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    ChangeName(CommandParams&& params);
};

//------------------------------------------------------------------------------
class List : public Command {
public:
    ~List() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    List(CommandParams&& params);
};

//------------------------------------------------------------------------------
class Whisper : public Command {
public:
    ~Whisper() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    Whisper(CommandParams&& params);

    std::string m_name;
};

//------------------------------------------------------------------------------
class Help : public Command {
public:
    ~Help() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    Help(CommandParams&& params);
};

//------------------------------------------------------------------------------
class Quit : public Command {
public:
    ~Quit() override = default;

    void execute() override;

    static size_t key;

private:
    friend Command;
    Quit(CommandParams&& params);
};

} // namespace ilrd

#endif // ILRD_COMMAND_HPP
