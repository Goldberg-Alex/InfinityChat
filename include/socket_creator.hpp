//implementaion of a class that listens to an open TCP socket
// and approves connection requests.

// version 1.0

// changelog
// 1.0 - created

#ifndef ILRD_SOCKET_CREATOR_HPP
#define ILRD_SOCKET_CREATOR_HPP

#include <string> //std::string

#include "socket.hpp" //Socket
//------------------------------------------------------------------------------

namespace ilrd
{
class SocketCreator
{
  public:
    explicit SocketCreator(std::string port);
    SocketCreator(SocketCreator &&) = default;
    SocketCreator &operator=(SocketCreator &&) = default;

    ~SocketCreator();

    Socket connect() const;
    int get_fd() const;

    SocketCreator(const SocketCreator &) = delete;
    SocketCreator &operator=(const SocketCreator &) = delete;

  private:
    int m_listen_socket;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SOCKET_CREATOR_HPP
