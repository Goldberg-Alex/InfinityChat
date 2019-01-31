// semaphore wrapper

#ifndef ILRD_SEMAPHORE_HPP
#define ILRD_SEMAPHORE_HPP

#include <semaphore.h> //semaphore

//------------------------------------------------------------------------------

namespace ilrd {

class Semaphore {
public:
    explicit Semaphore(unsigned int value = 0); // throws std::runtime_error
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    ~Semaphore();

    void post();
    void wait();
    void trywait();
    void timedwait(const struct timespec* abs_timeout);

private:
    sem_t m_sem;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SEMAPHORE_HPP
//
