// A request engine listening to file descriptors,
// receiving requests from them, and executing tasks for dealing with them.

// version 1.0

#ifndef ILRD_REQUEST_ENGINE_HPP
#define ILRD_REQUEST_ENGINE_HPP

#include <unordered_map> //std::unordered_map

#include "epoll.hpp"
#include "factory.hpp"
#include "plugnplay.hpp"
#include "thread_pool.hpp"
//------------------------------------------------------------------------------

namespace ilrd {

template <typename Key, typename TaskParams>
class RequestEngineGateway;

template <typename Key, typename TaskParams>
class RequestEngineRequest;

class RequestEngineTask;

template <typename Key, typename TaskParams>
class RequestEngine {
public:
    RequestEngine(std::string plugnplay_dir_path,
                  size_t num_threads = std::thread::hardware_concurrency()
                                           ? std::thread::hardware_concurrency()
                                           : 1);
    ~RequestEngine();

    using CreatorFunc =
        std::function<std::unique_ptr<RequestEngineTask>(TaskParams)>;

    void teach_task(const Key& key, CreatorFunc);
    void add_gateway(RequestEngineGateway<Key, TaskParams>* gateway);

    void set_num_threads(size_t num_threads);

    void run();

    RequestEngine(RequestEngine&&) = delete;
    RequestEngine& operator=(RequestEngine&&) = delete;

    RequestEngine(const RequestEngine&) = delete;
    RequestEngine& operator=(const RequestEngine&) = delete;

private:
    Epoll m_epoll;
    std::unordered_map<int, RequestEngineGateway<Key, TaskParams>*> m_gateways;
    ThreadPool m_thread_pool;
    DLLLoader m_loader;
    DirMonitor m_monitor;
};

class RequestEngineTask : public ThreadPoolTask {
public:
    enum class Priority { HIGH = 2, NORMAL = 3, LOW = 4 };

    explicit RequestEngineTask(Priority priority = Priority::NORMAL)
        : ThreadPoolTask(static_cast<unsigned int>(priority))
    {}
    virtual ~RequestEngineTask() = default;

    RequestEngineTask(RequestEngineTask&&) = default;
    RequestEngineTask& operator=(RequestEngineTask&&) = default;

    RequestEngineTask(const RequestEngineTask&) = delete;
    RequestEngineTask& operator=(const RequestEngineTask&) = delete;

private:
    virtual size_t execute() noexcept override = 0;
};

template <typename Key, typename TaskParams>
class RequestEngineRequest {
public:
    RequestEngineRequest(Key key, TaskParams params, bool to_stop);
    ~RequestEngineRequest();

    Key get_key();
    TaskParams& get_params();
    bool is_stop();

    RequestEngineRequest(RequestEngineRequest&&) = default;
    RequestEngineRequest& operator=(RequestEngineRequest&&) = default;

    RequestEngineRequest(const RequestEngineRequest&) = delete;
    RequestEngineRequest& operator=(const RequestEngineRequest&) = delete;

private:
    bool m_to_stop;
    Key m_key;
    TaskParams m_user_params;
};

template <typename Key, typename TaskParams>
class RequestEngineGateway {
public:
    explicit RequestEngineGateway(int fd);
    virtual ~RequestEngineGateway() = default;

    virtual std::unique_ptr<RequestEngineRequest<Key, TaskParams> >
    create() = 0;
    int get_fd() const;

protected:
    void set_fd(int fd);

private:
    int m_fd;
};

//------------------------------------------------------------------------------

template <typename Key, typename TaskParams>
RequestEngine<Key, TaskParams>::RequestEngine(std::string plugnplay_dir_path,
                                              size_t num_threads)
    : m_thread_pool(num_threads), m_monitor(plugnplay_dir_path)
{
    m_loader.register_dispatcher(m_monitor.get_dispatcher());
}

template <typename Key, typename TaskParams>
RequestEngine<Key, TaskParams>::~RequestEngine()
{
    LOG(INFO, "Destroyed Request Engine.");
}
template <typename Key, typename TaskParams>
void RequestEngine<Key, TaskParams>::teach_task(const Key& key,
                                                CreatorFunc create)
{
    auto task_factory =
        Handleton<Factory<RequestEngineTask, Key, TaskParams> >::get_instance();
    task_factory->add(key, create);
}

template <typename Key, typename TaskParams>
void RequestEngine<Key, TaskParams>::add_gateway(
    RequestEngineGateway<Key, TaskParams>* gateway)
{
    m_gateways[gateway->get_fd()] = gateway;

    m_epoll.add(gateway->get_fd(), EPOLLIN);
    LOG(INFO, "Added gateway to engine.");
}

template <typename Key, typename TaskParams>
void RequestEngine<Key, TaskParams>::set_num_threads(size_t num_threads)
{
    m_thread_pool.set_size(num_threads);
}

template <typename Key, typename TaskParams>
void RequestEngine<Key, TaskParams>::run()
{
    auto task_factory =
        Handleton<Factory<RequestEngineTask, Key, TaskParams> >::get_instance();
    bool stop = false;

    while (!stop) {
        int num_events = m_epoll.wait(-1);
        LOG(DEBUG, "exited epoll wait");

        for (int i = 0; i < num_events; i++) {

            auto request(m_gateways.at(m_epoll[i].m_fd)->create());

            if (request->is_stop()) {
                stop = true;
                break;
            }

            auto task(task_factory->create(request->get_key(),
                                           std::move(request->get_params())));

            m_thread_pool.add(std::move(task));
        }
    }
}

//------------------------------------------------------------------------------

template <typename Key, typename TaskParams>
RequestEngineGateway<Key, TaskParams>::RequestEngineGateway(int fd) : m_fd(fd)
{}

template <typename Key, typename TaskParams>
int RequestEngineGateway<Key, TaskParams>::get_fd() const
{
    return (m_fd);
}

//------------------------------------------------------------------------------

template <typename Key, typename TaskParams>
RequestEngineRequest<Key, TaskParams>::RequestEngineRequest(Key key,
                                                            TaskParams params,
                                                            bool to_stop)
    : m_to_stop(to_stop),
      m_key(key),
      m_user_params(std::forward<TaskParams>(params))
{}

template <typename Key, typename TaskParams>
RequestEngineRequest<Key, TaskParams>::~RequestEngineRequest()
{}

template <typename Key, typename TaskParams>
Key RequestEngineRequest<Key, TaskParams>::get_key()
{
    return (m_key);
}

template <typename Key, typename TaskParams>
TaskParams& RequestEngineRequest<Key, TaskParams>::get_params()
{
    return (m_user_params);
}
template <typename Key, typename TaskParams>
bool RequestEngineRequest<Key, TaskParams>::is_stop()
{
    return (m_to_stop);
}

//------------------------------------------------------------------------------

} // namespace ilrd

#endif // ILRD_REQUEST_ENGINE_HPP
