#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <mutex>
#include <fstream>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger
{
public:
    static Logger &getInstance();

    void init(const std::string &filePath);
    void log(LogLevel level, const std::string &message);
    void debug(const std::string &message);
    void info(const std::string &message);
    void warn(const std::string &message);
    void error(const std::string &message);

private:
    Logger() = default;
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::ofstream file;
    std::mutex mtx;
    static const char *levelToString(LogLevel level);
};

#ifdef RELEASE_MODE
#define LOG_DEBUG(msg) ((void)0)
#define LOG_INFO(msg) ((void)0)
#define LOG_WARN(msg) ((void)0)
#define LOG_ERROR(msg) ((void)0)
#else
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARN(msg) Logger::getInstance().warn(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#endif

#endif // LOGGER_H_
