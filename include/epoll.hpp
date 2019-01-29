// A wrapper class for the epoll system call family allowing RAII functionality

#ifndef ILRD_EPOLL_HPP
#define ILRD_EPOLL_HPP

#include <memory>      //std::unique_ptr
#include <sys/epoll.h> //epoll
#include <vector>      //vector

#include "code_utils.hpp"

//------------------------------------------------------------------------------

namespace ilrd {

class Epoll {
public:
    explicit Epoll(size_t max_events = 10);
    Epoll(Epoll&&) = default;
    Epoll& operator=(Epoll&&) = default;

    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;

    ~Epoll();

    // these functions throw runtime_error
    // add(STDIN, EPOLLIN | EPOLLHUP)
    void add(int fd, unsigned int events_type);
    void remove(int fd);
    int wait(int timeout);

    struct EventInfo {
        EventInfo(int fd, unsigned int event_type)
            : m_fd(fd), m_event_type(event_type)
        {}
        int m_fd;
        unsigned int m_event_type;
    };

    EventInfo operator[](size_t index);

private:
    size_t m_max_events;
    // pointer must be before the handle
    std::vector<epoll_event> m_events;
    int m_epoll_handle;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_EPOLL_HPP
