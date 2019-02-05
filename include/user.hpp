// TODO document

#ifndef ILRD_CLIENT_HPP
#define ILRD_CLIENT_HPP
//------------------------------------------------------------------------------

#include <string> // string

#include "socket.hpp" //Socket

//------------------------------------------------------------------------------

namespace ilrd {
class User {
public:
    explicit User(Socket&& sock);

    void set_name(const std::string& name);
    int get_fd() const;
    const std::string& get_name() const;

private:
    const Socket m_socket;
    std::string m_name;
};

} // namespace ilrd

#endif // ILRD_CLIENT_HPP
