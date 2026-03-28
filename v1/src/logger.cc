
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "../include/logger.h"

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    if (file.is_open())
        file.close();
}

void Logger::init(const std::string &filePath)
{
    std::lock_guard<std::mutex> lock(mtx);
    file.open(filePath, std::ios::app);
    if (!file.is_open())
        std::cerr << "Failed to open log file: " << filePath << "\n";
}

const char *Logger::levelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << " [" << levelToString(level) << "] "
        << message << "\n";

    std::string entry = oss.str();

    if (file.is_open())
    {
        file << entry;
        file.flush();
    }
    else
    {
        std::cout << entry;
    }
}

void Logger::debug(const std::string &message) { log(LogLevel::DEBUG, message); }
void Logger::info(const std::string &message) { log(LogLevel::INFO, message); }
void Logger::warn(const std::string &message) { log(LogLevel::WARN, message); }
void Logger::error(const std::string &message) { log(LogLevel::ERROR, message); }
