// an implementation of a thread pool receiving tasks
// and executing them in parallel.
// the number of threads can be adjusted at runtime.

// version 1.0

#ifndef ILRD_THREAD_POOL_HPP
#define ILRD_THREAD_POOL_HPP

#include <memory> //std::unique_ptr
#include <mutex>  //std::mutex
#include <thread> //std::thread
#include <vector> //std::vector

#include "wpqueue.hpp"

namespace ilrd {

// an abstract class defining the requirements of the thread pool.
// note that inserting a task with a priority of SYSTEM causes undefined
// behaviour.

class ThreadPoolTask {
public:
   
    explicit ThreadPoolTask(unsigned int priority = 3);
    virtual ~ThreadPoolTask();

    bool operator<(const ThreadPoolTask& other) const;

    ThreadPoolTask(ThreadPoolTask&&) = default;
    ThreadPoolTask& operator=(ThreadPoolTask&&) = default;

    ThreadPoolTask(const ThreadPoolTask&) = delete;
    ThreadPoolTask& operator=(const ThreadPoolTask&) = delete;

private:
    unsigned int m_priority;

    friend class ThreadPool;

    // the return value of execute is ignored
    virtual size_t execute() noexcept = 0;
};

class ThreadPool {
private:
    static constexpr size_t default_num_threads = 4;

    class StopThread;
    class CleanupThread;

public:
    explicit ThreadPool(size_t num_threads = default_num_threads);
    ~ThreadPool();

    void add(std::unique_ptr<ThreadPoolTask> task);

    void set_size(size_t size_threads);
    size_t get_size() const;

    // stops the thread pool and removes all the threads.
    // Waits up to timeout seconds for all enqueued tasks to be completed.
    // Any running tasks are allowed to complete before returning.
    // Any tasks remaining after timeout are discarded.
    void stop(std::chrono::seconds timeout);

    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    void run_tasks();
    WPQueue<std::unique_ptr<ThreadPoolTask> > m_pqueue;
    std::vector<std::thread> m_vec;
    mutable std::mutex m_mutex;
};

} // namespace ilrd

#endif // ILRD_THREAD_POOL_HPP
