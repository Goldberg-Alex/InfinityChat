//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// implementation of the SocketListener class
#include <netinet/in.h> //sockaddr_in
#include <sys/socket.h> //socket()

#include <cstring>   //std::memset()
#include <stdexcept> //std::runtime_error
#include <sys/un.h>  //sockaddr_un

#include "config.hpp"          //tcp_backlog_size
#include "logger.hpp"          //Logger
#include "socket_listener.hpp" //SocketListener

//------------------------------------------------------------------------------
namespace ilrd {
Socket get_listen_socket(std::string port);
//------------------------------------------------------------------------------

SocketListener::SocketListener(std::string port)
    : m_listen_socket(get_listen_socket(port))
{}

//------------------------------------------------------------------------------
Socket SocketListener::connect() const
{
    struct sockaddr_un peer_addr;
    std::memset(&peer_addr, 0, sizeof(struct sockaddr_un));

    socklen_t peer_addr_size = sizeof(peer_addr);

    int socket = accept(m_listen_socket.get_fd(),
                        (struct sockaddr*)&peer_addr,
                        &peer_addr_size);

    if (-1 == socket) {
        throw std::runtime_error("error connecting socket");
    }

    LOG(INFO, "connected new socket.");
    return (Socket(socket));
}

int SocketListener::get_fd() const
{
    return (m_listen_socket.get_fd());
}

Socket get_listen_socket(std::string port)
{
    int opt = 1;
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!listen_socket) {
        throw std::runtime_error("error opening listen socket.");
    }

    Socket result(listen_socket);

    int status = setsockopt(listen_socket,
                            SOL_SOCKET,
                            SO_REUSEADDR | SO_REUSEPORT,
                            &opt,
                            sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(stoul(port));

    status |= bind(listen_socket, (struct sockaddr*)&address, sizeof(address));

    status |= listen(listen_socket, tcp_backlog_size);

    // we dont care which function failed, just throw an exception.
    // the socket will be closed by the Socket class
    if (status) {
        throw std::runtime_error("error opening listen socket.");
    }

    return (result);
}

} // namespace ilrd
//------------------------------------------------------------------------------
