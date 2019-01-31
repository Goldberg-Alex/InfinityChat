// TODO write explanations

#ifndef ILRD_CLIENT_HPP
#define ILRD_CLIENT_HPP

#include <string> // string

#include "command.hpp"

namespace ilrd {
std::unique_ptr<Command> parse(std::string command, User& user);
void produce_commands();
void consume_commands(std::deque<std::unique_ptr<Command> >& command_q);

} // namespace ilrd

#endif // ILRD_CLIENT_HPP
