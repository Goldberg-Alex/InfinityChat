// Implementtion of the socket class used by both client and server
//must receive an open socket. closes the socket on destruction

// version 1.0

// changelog
// 1.0 - created

#ifndef ILRD_SOCKET_HPP
#define ILRD_SOCKET_HPP

#include <sys/types.h> //size_t
#include <string>      //std::string

//------------------------------------------------------------------------------

namespace ilrd
{
class Socket
{
public:
  //receives an open socket
  explicit Socket(int fd);
  Socket(Socket &&) = default;
  Socket &operator=(Socket &&) = default;

  ~Socket();

  void send(const std::string &message) const;
  const std::string &receive() const;

  int get_fd();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

private:
  int m_fd;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SOCKET_HPP
