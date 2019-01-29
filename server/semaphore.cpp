//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			semaphore.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// A wrapper to a posix semaphore providing RAII.

#include "code_utils.hpp"

#include "semaphore.hpp"

//------------------------------------------------------------------------------
namespace ilrd {
Semaphore::Semaphore(unsigned int value)
{
    if (sem_init(&m_sem, 0, value)) {
        throw std::runtime_error("initializing semaphore failed.");
    }
}

Semaphore::~Semaphore()
{
    // if some thread is waiting in wait() when we call destroy.
    // behaviour is undefined.

    sem_destroy(&m_sem);
}
void Semaphore::post()
{
    sem_post(&m_sem);
}
void Semaphore::wait()
{
    sem_wait(&m_sem);
}
void Semaphore::trywait()
{
    sem_trywait(&m_sem);
}
void Semaphore::timedwait(const struct timespec* abs_timeout)
{
    sem_timedwait(&m_sem, abs_timeout);
}
} // namespace ilrd
