#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include <mutex>
#include <string.h>

#define PRINT_LOG
#define RTDS_CLI_MODE
using namespace boost;

class Log
{
    static std::ofstream logFile;
    static bool goodToLog;
    static std::mutex writeLock;
    static void _printTime();
public:
    static void startLog(std::string);
    static void log(std::string, const std::runtime_error&);
    static void log(std::string, const system::error_code&);
    static void log(std::string, const asio::ip::tcp::socket*);
    static void log(std::string);
    static void stopLog();
};

