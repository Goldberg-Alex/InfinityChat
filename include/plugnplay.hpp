// Implementation of a plug and play mechanism allowing loading shared objects
// at runtime.
// version 1.0

#ifndef ILRD_PLUGNPLAY_HPP
#define ILRD_PLUGNPLAY_HPP

#include <atomic> //std::atomic
#include <string> //std::string
#include <thread> //std:thread

#include "dispatcher_callback.hpp"

namespace ilrd {

class DirMonitor {
public:
    explicit DirMonitor(const std::string& path);
    ~DirMonitor();

    Dispatcher<std::string>* get_dispatcher();

    DirMonitor(DirMonitor&&) = delete;
    DirMonitor& operator=(DirMonitor&&) = delete;
    DirMonitor(const DirMonitor&) = delete;
    DirMonitor& operator=(const DirMonitor&) = delete;

private:
    void monitor();

    Dispatcher<std::string> m_disp;
    std::string m_path;
    int m_inotify_fd;
    std::atomic<bool> m_is_running;
    std::thread m_thread;
};

class DLLLoader {
public:
    explicit DLLLoader(Dispatcher<std::string>* dispatcher = nullptr);
    ~DLLLoader() = default;

    void register_dispatcher(Dispatcher<std::string>* dispatcher);

    void dll_load(const std::string& filename);
    void stop();

    DLLLoader(DLLLoader&&) = delete;
    DLLLoader& operator=(DLLLoader&&) = delete;
    DLLLoader(const DLLLoader&) = delete;
    DLLLoader& operator=(const DLLLoader&) = delete;

private:
    CallBack<std::string, DLLLoader> m_call;
};

} // namespace ilrd

#endif // ILRD_PLUGNPLAY_HPP
