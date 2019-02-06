//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is an implementation of a logger class
#include <chrono>  //std::chrono::system_clock::now()
#include <ctime>   //std::localtime()
#include <iomanip> //std::put_time()

#include "config.hpp" //log_level, log_filename
#include "logger.hpp"
#include "scopelock.hpp"
//------------------------------------------------------------------------------
namespace ilrd {

void Logger::write(enum log_level log_lvl,
                   const std::string& message,
                   std::string&& filename,
                   int line,
                   std::string&& function)
{
    if (log_lvl > m_log_level) {
        return;
    }

    std::string log_lvl_str;
    if (ERROR == log_lvl) {
        log_lvl_str = "ERROR";

    } else if (INFO == log_lvl) {
        log_lvl_str = "INFO";

    } else {
        log_lvl_str = "DEBUG";
    }

    auto now = std::time(nullptr);
    ScopeLock<std::mutex> lock(m_mutex);

    // TODO maybe combine all these into a signle string and insert once?
    m_stream << "["<<log_lvl_str << "] ["
             << std::put_time(std::localtime(&now), "%F | %T") << "]"
             << " [" << filename.substr(filename.find_last_of('/') + 1) << " : "
             << line << " : " << function << "] " << message << std::endl;
}

Logger::Logger(std::string path, enum log_level log_lvl)
    : m_log_level(log_lvl), m_stream(path)
{}

} // namespace ilrd
//------------------------------------------------------------------------------
