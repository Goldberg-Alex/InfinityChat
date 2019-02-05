//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-		    socket.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// Implementation of the Socket class

#include <cstring>   //memset()
#include <stdexcept> //std::runtime_error
#include <unistd.h>  //close(), read(), write()

#include "../include/socket.hpp"
//------------------------------------------------------------------------------
namespace ilrd {
Socket::Socket(int fd) : m_fd(fd)
{}

Socket::~Socket()
{
    close(m_fd);
}

void Socket::send(const std::string& message) const
{
    int written_bytes = write(m_fd, message.c_str(), message.size());

    if (-1 == written_bytes) {
        throw std::runtime_error("error writing to socket");
    }
}

const std::string& Socket::receive() const
{
    static const int BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE];
    std::string result;

    int read_bytes;

    do {
        memset(buffer, 0, BUFFER_SIZE);
        read_bytes = read(m_fd, buffer, BUFFER_SIZE);
        if (-1 == read_bytes) {
            throw std::runtime_error("error reading from socket");
        }

        result += buffer;

    } while (read_bytes == BUFFER_SIZE);

    return (std::move(result));
}

int Socket::get_fd() const
{
    return (m_fd);
}
//------------------------------------------------------------------------------

} // namespace ilrd
