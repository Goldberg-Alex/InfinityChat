// definitions for an observer-side CallBack and subject-side dispatcher.
// the dispatcher sends messages to all registered callbacks.

// version 1.0

#ifndef ILRD_OBSERVER_DISPATCHER_HPP
#define ILRD_OBSERVER_DISPATCHER_HPP

#include <algorithm> //std::find
#include <list>      //std:list

namespace ilrd {

template <typename Msg>
class CallBackBase;

template <typename Msg>
class Dispatcher {
public:
    Dispatcher() = default;
    ~Dispatcher();

    // The API for the subject is public.
    void notify_all(const Msg& message);

    Dispatcher(Dispatcher&&) = delete;
    Dispatcher& operator=(Dispatcher&&) = delete;

    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

private:
    // The API for the CallBackBase is private.
    friend class CallBackBase<Msg>;
    void add(CallBackBase<Msg>* callback);
    void remove(CallBackBase<Msg>* callback);

    bool m_is_destroyed = false;
    std::list<CallBackBase<Msg>*> m_callbacks;
};

template <typename Msg>
class CallBackBase {
public:
    CallBackBase() = default;
    virtual ~CallBackBase();

    // The API for the observer is public.
    // sets the dispatcher to the given dispatcher.
    // we disconnect from the previous dispatcher before registering to
    // the new one.
    // set dispatcher to null to just disconnect from the currrent dispatcher.
    void register_dispatcher(Dispatcher<Msg>* dispatcher);

    CallBackBase(CallBackBase&&) = delete;
    CallBackBase& operator=(CallBackBase&&) = delete;

    CallBackBase(const CallBackBase&) = delete;
    CallBackBase& operator=(const CallBackBase&) = delete;

private:
    // The API for the Dispatcher is private.
    friend class Dispatcher<Msg>;
    virtual void notify(const Msg& message) = 0;
    virtual void notify_service_out() = 0;

    // initialisation list
    Dispatcher<Msg>* m_disp = {nullptr};
};

template <typename Msg, typename Observer>
class CallBack final : public CallBackBase<Msg> {
public:
    using ActionMethod = void (Observer::*)(const Msg&);
    using StopMethod = void (Observer::*)();

    CallBack(Observer& observer,
             ActionMethod action,
             StopMethod stop_action = nullptr);
    ~CallBack() override = default;

    void notify(const Msg& message) override;
    void notify_service_out() override;

private:
    Observer& m_observer;
    ActionMethod m_action_func;
    StopMethod m_stop_func;
};

//------------------------------------------------------------------------------

template <typename Msg>
Dispatcher<Msg>::~Dispatcher()
{
    m_is_destroyed = true;
    while (m_callbacks.begin() != m_callbacks.end()) {
        (*m_callbacks.begin())->notify_service_out();
    }
}

template <typename Msg>
void Dispatcher<Msg>::notify_all(const Msg& message)
{
    // Save the next iterator before advancing to handle the case of
    // invalidating the iterator.

    for (auto i = m_callbacks.begin(); i != m_callbacks.end();) {
        auto i_next = std::next(i);

        (*i)->notify(message);

        i = i_next;
    }
} // namespace ilrd

template <typename Msg>
void Dispatcher<Msg>::add(CallBackBase<Msg>* callback)
{
    if (!m_is_destroyed) {
        m_callbacks.push_back(callback);
    }
}

template <typename Msg>
void Dispatcher<Msg>::remove(CallBackBase<Msg>* callback)
{
    m_callbacks.remove(callback);
}

//------------------------------------------------------------------------------

template <typename Msg>
CallBackBase<Msg>::~CallBackBase()
{
    if (m_disp) {
        m_disp->remove(this);
    }
}

template <typename Msg>
void CallBackBase<Msg>::register_dispatcher(Dispatcher<Msg>* dispatcher)
{
    if (m_disp) {
        m_disp->remove(this);
    }

    if (dispatcher) {
        dispatcher->add(this);
    }

    m_disp = dispatcher;
}

//------------------------------------------------------------------------------

template <typename Msg, typename Observer>
CallBack<Msg, Observer>::CallBack(Observer& observer,
                                  ActionMethod action,
                                  StopMethod stop_action)
    : m_observer(observer), m_action_func(action), m_stop_func(stop_action)
{}

template <typename Msg, typename Observer>
void CallBack<Msg, Observer>::notify(const Msg& message)
{
    (m_observer.*m_action_func)(message);
}

template <typename Msg, typename Observer>
void CallBack<Msg, Observer>::notify_service_out()
{
    CallBackBase<Msg>::register_dispatcher(nullptr);

    if (m_stop_func) {
        (m_observer.*m_stop_func)();
    }
}

//------------------------------------------------------------------------------

} // namespace ilrd

#endif // ILRD_OBSERVER_DISPATCHER_HPP
