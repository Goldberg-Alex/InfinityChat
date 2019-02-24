// Implementtion of the socket class used by both client and server
// must receive an open socket. closes the socket on destruction

// version 1.0

// changelog
// 1.0 - created

#ifndef ILRD_SOCKET_HPP
#define ILRD_SOCKET_HPP

#include <string>      //std::string
#include <sys/types.h> //size_t

//------------------------------------------------------------------------------

namespace ilrd {
class Socket {
public:
    // receives an open socket
    explicit Socket(int fd);
    Socket(Socket&&);
    Socket& operator=(Socket&&);

    ~Socket();

    // throws std::logic_error, std::runtime_error
    static Socket create(const std::string& ip_address,
                         const std::string& port);

    void send(const std::string& message) const;
    std::string receive() const;

    int get_fd() const;

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

private:
    int m_fd;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SOCKET_HPP
