// TODO write explanations

#ifndef ILRD_CLIENT_HPP
#define ILRD_CLIENT_HPP

#include <string> // string

namespace ilrd {
class User {
public:
    explicit User(int fd);

    void set_name(const std::string& name);
    int get_fd() const;
    std::string& get_name() const;

private:
    const int m_fd;
    std::string m_name;
};

} // namespace ilrd

#endif // ILRD_CLIENT_HPP
