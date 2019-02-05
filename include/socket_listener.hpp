// implementaion of a class that listens to an open TCP socket
// and approves connection requests.

// version 1.0

// changelog
// 1.0 - created

#ifndef ILRD_SOCKET_LISTENER_HPP
#define ILRD_SOCKET_LISTENER_HPP

#include <string> //std::string

#include "socket.hpp" //Socket
//------------------------------------------------------------------------------

namespace ilrd {
class SocketListener {
public:
    explicit SocketListener(std::string port);

    SocketListener(SocketListener&&) = default;
    SocketListener& operator=(SocketListener&&) = default;

    ~SocketListener() = default;

    Socket connect() const;
    int get_fd() const;

    SocketListener(const SocketListener&) = delete;
    SocketListener& operator=(const SocketListener&) = delete;

private:
    Socket m_listen_socket;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SOCKET_LISTENER_HPP
