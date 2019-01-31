// the API functions of both the input and output parts of the server.

#ifndef ILRD_CLIENT_HPP
#define ILRD_CLIENT_HPP

#include <string> // std::string
#include <memory> //std::unique_ptr
#include <deque>  //std::queue

#include "command.hpp"

namespace ilrd
{
std::unique_ptr<Command> parse(std::string command, User &user);
void produce_commands();
void consume_commands(std::deque<std::unique_ptr<Command>> &command_q);
} // namespace ilrd

#endif // ILRD_CLIENT_HPP
