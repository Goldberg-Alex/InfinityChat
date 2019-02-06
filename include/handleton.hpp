// example:
// std::string& i = Handleton<std::string>::get_instance();
// std::string& j = Handleton<std::string>::get_instance();
// i == j; //this will always be true

// NOTE: the pointer returned by get_instance should not be delete'd.

#ifndef ILRD_HANDLETON_HPP
#define ILRD_HANDLETON_HPP

#include <atomic>  //std::atomic
#include <cstdlib> //std::Atexit()
#include <mutex>   //std::mutex

#include "scopelock.hpp"
namespace ilrd {

template <typename T>
class Handleton {
public:
    template <typename... Args>
    static T* get_instance(Args... args);

private:
    Handleton() = delete;
    ~Handleton() = delete;
    Handleton(Handleton&&) = delete;
    Handleton& operator=(Handleton&&) = delete;
    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;
    static void destroy();

    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
};

template <typename T>
void Handleton<T>::destroy()
{
    delete s_instance;
    s_instance = nullptr;
}

template <typename T>
std::atomic<T*> Handleton<T>::s_instance;

template <typename T>
std::mutex Handleton<T>::s_mutex;

template <typename T>
template <typename... Args>
T* Handleton<T>::get_instance(Args... args)
{
    T* tmp = s_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);

    if (!tmp) {
        ScopeLock<std::mutex> lock(s_mutex);
        tmp = s_instance.load(std::memory_order_relaxed);

        if (!tmp) {
            tmp = new T(args...);
            std::atomic_thread_fence(std::memory_order_release);
            s_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}


#define INIT_HANDLETON(type)                                                   \
    template <>                                                                \
    std::atomic<type*> Handleton<type>::s_instance(nullptr);                   \
    template <>                                                                \
    std::mutex Handleton<type>::s_mutex;
} // namespace ilrd

#endif // ILRD_HANDLETON_HPP
