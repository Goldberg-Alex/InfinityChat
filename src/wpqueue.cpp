//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// implementation of wpqueue

#include "wpqueue.hpp"
#include "scopelock.hpp"
//------------------------------------------------------------------------------
namespace ilrd {
template <typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::push(const T& data)
{
    push(std::move(data));
}

template <typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::push(T&& data)
{
    ScopeLock<std::mutex> lock(m_mutex);
    m_queue.push(data);
    m_sem.post();
}

//------------------------------------------------------------------------------
template <typename T, typename Container, typename Compare>
template <typename TimeUnit>
bool WPQueue<T, Container, Compare>::pop(T& data, TimeUnit timeout)
{
    m_sem.timedwait(timeout);
    ScopeLock<std::mutex> lock(m_mutex);

    if (is_empty()) {
        return false;
    }

    data = m_queue.top();

    m_queue.pop();

    return true;
}

template <typename T, typename Container, typename Compare>
void WPQueue<T, Container, Compare>::pop(T& data)
{
    m_sem.wait();
    ScopeLock<std::mutex> lock(m_mutex);

    data = m_queue.top();

    m_queue.pop();
}
//------------------------------------------------------------------------------

template <typename T, typename Container, typename Compare>
bool WPQueue<T, Container, Compare>::is_empty() const
{
    return (m_queue.empty());
}

template <typename T, typename Container, typename Compare>
size_t WPQueue<T, Container, Compare>::size() const
{
    return (m_queue.size());
}

} // namespace ilrd
//------------------------------------------------------------------------------
