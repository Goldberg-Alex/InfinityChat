//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// implementation of the Epoll class
#include <unistd.h> //close

#include "epoll.hpp"
//------------------------------------------------------------------------------
namespace ilrd {

Epoll::Epoll(size_t max_events)
    : m_max_events(max_events),
      m_events((max_events)),
      m_epoll_handle(epoll_create1(0))
{
    if (m_epoll_handle < 0) {
        throw std::runtime_error("Initializing Epoll failed.");
    }
}

Epoll::~Epoll()
{
    close(m_epoll_handle);
}

//------------------------------------------------------------------------------

void Epoll::add(int fd, unsigned int events_type)
{
    static struct epoll_event event;

    event.data.fd = fd;
    event.events = events_type;

    if (0 != epoll_ctl(m_epoll_handle, EPOLL_CTL_ADD, event.data.fd, &event)) {
        throw std::runtime_error("Error adding event to epoll.");
    }
}

void Epoll::remove(int fd)
{
    if (0 != epoll_ctl(m_epoll_handle, EPOLL_CTL_DEL, fd, NULL)) {
        throw std::runtime_error("Error removing event to epoll.");
    }
}

int Epoll::wait(int timeout)
{
    return (epoll_wait(m_epoll_handle,
                       m_events.data(),
                       static_cast<int>(m_max_events),
                       timeout));
}

Epoll::EventInfo Epoll::operator[](size_t index)
{
    return (EventInfo(m_events[index].data.fd, m_events[index].events));
}

} // namespace ilrd
//------------------------------------------------------------------------------
