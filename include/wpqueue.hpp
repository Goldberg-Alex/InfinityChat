// a thread-safe wrapper for a priority queue.
// poping is a blocking operation if the queue is empty.
// contained type must provide operator<.

// version 1.0

#ifndef ILRD_WPQUEUE_HPP
#define ILRD_WPQUEUE_HPP

#include <chrono> //std::chrono::duration
#include <mutex>  //std::mutex
#include <queue>  //std::priority_queue

#include "semaphore.hpp"

namespace ilrd {
template <typename T,
          typename Container = std::vector<T>,
          typename Compare = std::less<typename Container::value_type> >
class WPQueue {
public:
    WPQueue() = default;
    ~WPQueue() = default;

    void push(const T& data);
    void push(T&& data);

    using MicroSeconds = std::chrono::duration<size_t, std::micro>;

    // waits for timeout. 0 value means dont wait at all.

    // return value:
    // false for timed out.
    // true for popped successfully.
    template <typename TimeUnit = MicroSeconds>
    bool pop(T& data, TimeUnit timeout);

    // waits forever
    void pop(T& data);

    bool is_empty() const;
    size_t size() const;

    WPQueue(WPQueue&&) = delete;
    WPQueue& operator=(WPQueue&&) = delete;
    WPQueue(const WPQueue&) = delete;
    WPQueue& operator=(const WPQueue&) = delete;

private:
    std::priority_queue<T, Container, Compare> m_queue;
    Semaphore m_sem;
    std::mutex m_mutex;
};

} // namespace ilrd

#endif // ILRD_WPQUEUE_HPP
