// A logger class allowing logging to file.
// must be used inside a singleton

#ifndef ILRD_LOGGER_HPP
#define ILRD_LOGGER_HPP

#include <fstream> //std::ofstream
#include <mutex>   //std::mutex
#include <string>  //std::string

#include "config.hpp" //LOG_LEVEL, LOG_FILENAME
#include "handleton.hpp"
//------------------------------------------------------------------------------

namespace ilrd {

class Logger {
public:

    void write(enum log_level log_level,
               const std::string& message,
               std::string&& filename,
               int line,
               std::string&& function_name);

private:
    Logger(std::string path = LOG_FILENAME, enum log_level log_lvl = LOG_LEVEL);

    ~Logger() = default;

    Logger(Logger&&) = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    enum log_level m_log_level;
    std::ofstream m_stream;
    std::mutex m_mutex;

    friend class Handleton<Logger>;
};

#define LOG(level, message)                                                    \
    do {                                                                       \
        Handleton<Logger>::get_instance()->write(Logger::log_level::level,     \
                                                 message,                      \
                                                 __FILE__,                     \
                                                 __LINE__,                     \
                                                 __FUNCTION__);                \
    } while (0);

} // namespace ilrd

//------------------------------------------------------------------------------

#endif // ILRD_LOGGER_HPP
