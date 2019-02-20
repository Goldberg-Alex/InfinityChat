//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-		    socket.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// Implementation of the Socket class

#include <arpa/inet.h>  // inet_pton
#include <cstring>      //memset()
#include <cstring>      //memset
#include <iostream>     //cout
#include <stdexcept>    //std::runtime_error
#include <sys/socket.h> //socket()
#include <unistd.h>     //close(), read(), write()

#include "logger.hpp"
#include "socket.hpp"
//------------------------------------------------------------------------------
namespace ilrd {
Socket::Socket(int fd) : m_fd(fd)
{}

Socket::~Socket()
{
    close(m_fd);
}

Socket Socket::create(const std::string& ip_address, const std::string& port)
{
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == soc_fd) {

        std::string str("error creating socket");
        LOG(ERROR, str);
        throw std::runtime_error(str);
    }

    // create the result here for RAII if something is thrown later.
    Socket result(soc_fd);

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    //! does port need to be converted with htonl?
    serv_addr.sin_port = static_cast<unsigned short>(std::stoul(port));

    if (1 != inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr)) {
        std::string str("invalid ip address.");
        LOG(ERROR, str);
        throw std::runtime_error(str);
    }

    if (connect(soc_fd,
                reinterpret_cast<struct sockaddr*>(&serv_addr),
                sizeof(serv_addr)) < 0) {
        std::string str("connecting socket failed.");
        LOG(ERROR, str);
        throw std::runtime_error(str);
    }
    LOG(INFO, "connected socket successfully.")
    return (result);
}

void Socket::send(const std::string& message) const
{
    ssize_t written_bytes = write(m_fd, message.c_str(), message.size());
    std::cout << message << '\n';

    if (-1 == written_bytes) {
        std::string str("failed writing to socket");
        LOG(ERROR, str);
        throw std::runtime_error(str);
    }
}

std::string Socket::receive() const
{
    static const int BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE];
    std::string result;

    ssize_t read_bytes;

    do {
        memset(buffer, 0, BUFFER_SIZE);
        read_bytes = read(m_fd, buffer, BUFFER_SIZE);
        if (-1 == read_bytes) {
            throw std::runtime_error("error reading from socket");
        }

        try {
            result += buffer;

        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }

    } while (read_bytes);

    return (result);
}

int Socket::get_fd() const
{
    return (m_fd);
}
//------------------------------------------------------------------------------

} // namespace ilrd
