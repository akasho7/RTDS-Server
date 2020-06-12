#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string.h>
#include <asio.hpp>
#include <iostream>

#define START_LOG Log::startLog();
#define STOP_LOG Log::stopLog();

#ifdef PRINT_DEBUG_LOG
#define DEBUG_LOG(x) x
#else
#define DEBUG_LOG(x)
#endif

#ifdef PRINT_LOG
#define LOG(x) x
#else
#define LOG(x)
#endif

class Log
{
public:
/*******************************************************************************************
* @brief Enable logging
********************************************************************************************/
    static void startLog();
/*******************************************************************************************
* @brief Disable Logging
********************************************************************************************/
    static void stopLog();
/*******************************************************************************************
* @brief Check if the logging is enbaled or disabled
*
* @return                       True if the logging is enabled
********************************************************************************************/
    static bool isLogging();
/*******************************************************************************************
* @brief Print the message
*
* @param[in]        Message
********************************************************************************************/
    template<typename T, typename... Args>
    static void log(T, Args...);

private:
    static std::atomic_bool _needLog;           // True if needs logs
    static std::mutex _consoleWriteLock;        // Mutex for thread safety
};

template<typename T, typename... Args>
inline void Log::log(T message, Args... messages)
{
    std::lock_guard<std::mutex> lock(_consoleWriteLock);
    if (_needLog)
    {
        std::cout << "RTDS: ";
        std::cout << message;
        (std::cout << ... << std::forward<Args>(messages));
        std::cout << std::endl;
    }
}

#endif