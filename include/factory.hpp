// a generic factory class allowing different hierarchies of classes.
// version 1.0

#ifndef ILRD_FACTORY_HPP
#define ILRD_FACTORY_HPP

#include <functional>    //std::function
#include <memory>        //std::unique_ptr
#include <unordered_map> //std::unordered_map

namespace ilrd {
class create_error : public std::runtime_error {
public: 
    create_error(const std::string& err): std::runtime_error(err) {}
};

class bad_key : public std::logic_error {
public:
    bad_key(const std::string& err): std::logic_error(err) {}
};

// Base is the base class of the class family.
// key is a UNIQUE identifier for each derived class.
// params are the parameters to the creator methods.
template <typename Base, typename Key, typename... Args>
class Factory {
public:
    // example std::unique_ptr<Person> create_warrior(std::string name)
    using CreatorFunc = std::function<std::unique_ptr<Base>(Args...)>;

    Factory() = default;
    ~Factory() = default;

    enum add_result {
        ADDED = 0,
        REPLACED = 1,
    };

    add_result add(const Key& key, CreatorFunc creator); // throws (what?)

    std::unique_ptr<Base> create(const Key& key,
                                 Args... args); // throws create_error, bad_key

private:
    Factory(Factory&&) = delete;
    Factory& operator=(Factory&&) = delete;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    std::unordered_map<Key, CreatorFunc> m_map;
};

template <typename Base, typename Key, typename ... Args>
typename Factory<Base, Key, Args...>::add_result
Factory<Base, Key, Args...>::add(const Key& key, CreatorFunc creator)
{
    add_result result = (m_map.find(key) == m_map.end()) ? ADDED : REPLACED;
    m_map[key] = creator;

    return result;
}

template <typename Base, typename Key, typename ... Args>
std::unique_ptr<Base> Factory<Base, Key, Args...>::create(const Key& key,
                                                         Args... args)
{
    auto iter = m_map.find(key);
    if (iter == m_map.end()) {
        throw bad_key("bad key");
    }

    auto created = iter->second(args...);
    if (!created) {
        throw create_error("bad alloc");
    }

    return created;
}

} // namespace ilrd

#endif // ILRD_FACTORY_HPP
