//-----------------------------------------
// Written by - 		Evgeny
// File Name-           command.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the implementation of the command class and it's deriving classes.
// each type of command has it's own class with it's own execute() command.

//------------------------------------------------------------------------------
#include <algorithm> //std::for_each
#include <utility>   //std::forward

#include "command.hpp"
#include "logger.hpp"
namespace ilrd {

//------------------------------------------------------------------------------
size_t Message::key(0);
size_t ChangeName::key(0);
size_t List::key(0);
size_t Whisper::key(0);
size_t Help::key(0);
size_t Quit::key(0);
std::vector<std::string> Command::s_command_list;

//------------------------------------------------------------------------------
Command::Command(CommandParams&& params)
    : m_params(std::forward<CommandParams>(params))
{}

void Command::init_command_list()
{
    Message::key = push_get_index("/say");
    ChangeName::key = push_get_index("/name");
    List::key = push_get_index("/list");
    Whisper::key = push_get_index("/whisper");
    Help::key = push_get_index("/help");
    Quit::key = push_get_index("/quit");
}

size_t Command::push_get_index(std::string&& str)
{
    s_command_list.push_back(std::move(str));
    return (s_command_list.size() - 1);
}

const std::string& Command::get_args() const
{
    return (m_params.args);
}

const std::shared_ptr<User> Command::get_user() const
{
    return (m_params.user);
}
void Command::send_global_msg(const std::string& msg)
{
    for (auto&& iter = m_params.list.begin(); iter != m_params.list.end();
         ++iter) {

        iter->second->get_socket().send(msg);
    }
}
//------------------------------------------------------------------------------

Message::Message(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void Message::execute()
{
    LOG(DEBUG, "sending message: " + m_params.args);
    std::string msg(m_params.user->get_name() + ": " + m_params.args);

    send_global_msg(msg);
}

//------------------------------------------------------------------------------
ChangeName::ChangeName(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void ChangeName::execute()
{
    LOG(DEBUG,
        "changing name from " + m_params.user->get_name() + " to " +
            m_params.args);

    std::string old_name(m_params.user->get_name());

    m_params.list.change_name(m_params.user, m_params.args);
    if (m_params.user->get_name() != m_params.args) {
        // failed to change the name
        std::string str("Name " + m_params.args + " is taken");
        LOG(DEBUG, str);
        m_params.user->get_socket().send(str);
    } else {
        std::string msg(old_name + " changed name to " +
                        m_params.user->get_name());

        send_global_msg(msg);
    }
}

//------------------------------------------------------------------------------
List::List(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void List::execute()
{
    LOG(DEBUG, "sending list");
    std::string msg("Name list:\n");

    for (auto&& iter = m_params.list.begin(); iter != m_params.list.end();
         ++iter) {

        msg += iter->second->get_name() + "\n";
    }

    m_params.user->get_socket().send(msg);
}

//------------------------------------------------------------------------------
Whisper::Whisper(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void Whisper::execute()
{
    LOG(DEBUG, "sending whisper: " + m_params.args);

    size_t name_index = m_params.args.find_first_of(" ");
    std::shared_ptr<ilrd::User> callee;

    if (std::string::npos == name_index ||
        !(callee = m_params.list.find(m_params.args.substr(0, name_index)))) {

        LOG(DEBUG, "no user found for whisper");
        m_params.user->get_socket().send("no user found for whisper");
        return;
    }

    std::string whisper(m_params.args.substr(name_index + 1));

    callee->get_socket().send("(whisper)" + m_params.user->get_name() + ": " +
                              whisper);

    m_params.user->get_socket().send("(whisper to " + callee->get_name() +
                                     "): " + whisper);
}

//------------------------------------------------------------------------------
Help::Help(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void Help::execute()
{
    LOG(DEBUG, "sending help");
    std::string msg("Commands:\n");

    for (auto& i : Command::s_command_list) {
        msg += i + " ";
    }

    m_params.user->get_socket().send(msg);
}

//------------------------------------------------------------------------------
Quit::Quit(CommandParams&& params)
    : Command(std::forward<CommandParams>(params))
{}

void Quit::execute()
{
    LOG(DEBUG, "Removing user from the list");
    std::string msg("user " + m_params.user->get_name() + " disconnected.");

    m_params.epoll.remove(m_params.user->get_fd());
    m_params.list.remove(m_params.user->get_fd());

    send_global_msg(msg);
}

} // namespace ilrd
