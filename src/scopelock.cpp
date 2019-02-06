//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			scopelock.hpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program
#include <mutex>     //mutex, recursive_mutex
#include <pthread.h> //mutex

#include "scopelock.hpp"

//------------------------------------------------------------------------------
namespace ilrd {

template <typename T>
ScopeLock<T>::ScopeLock(T& mutex) : m_mutex(mutex)
{
    m_mutex.lock();
}

template <typename T>
ScopeLock<T>::~ScopeLock()
{
    m_mutex.unlock();
}
//------------------------------------------------------------------------------

template <>
ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t& mutex) : m_mutex(mutex)
{
    pthread_mutex_lock(&m_mutex);
}

template <>
ScopeLock<pthread_mutex_t>::~ScopeLock()
{
    pthread_mutex_unlock(&m_mutex);
}

template class ScopeLock<std::mutex>;
template class ScopeLock<std::recursive_mutex>;
template class ScopeLock<pthread_mutex_t>;

} // namespace ilrd
//------------------------------------------------------------------------------
