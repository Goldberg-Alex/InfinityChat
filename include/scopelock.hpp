// Implementation of a scope lock for mutex. supports std::mutex,
// std::recursive_mutex and pthread_mutex.

// The object locks the mutex when created and unlocks the mutex when destroyed.

// Using ScopeLock for a pthread_mutex might throw runtime_error if it fails.
// in that case the state of the mutex is undefined.

// Giving a locked mutex or an initialized pthread_mutex results in undefined
// behaviour.

// usage example

// std::mutex m;
// Scopelock<std::mutex> SL(m);

// pthread_mutex_t m;
// Scopelock<std::mutex> SL(&m);

#ifndef ILRD_SCOPELOCK_HPP
#define ILRD_SCOPELOCK_HPP

//------------------------------------------------------------------------------

namespace ilrd {

template <typename T>
class ScopeLock {
public:
    explicit ScopeLock(T& mutex);
    ~ScopeLock();

private:
    // uncopyable, anassignable
    ScopeLock(const ScopeLock&);
    ScopeLock& operator=(const ScopeLock&);

    T& m_mutex;
};

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_SCOPELOCK_HPP
