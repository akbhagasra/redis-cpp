#ifndef CONFIGS_H_
#define CONFIGS_H_

class Configs
{
public:
    static constexpr int PORT = 6379;
    static constexpr int BACKLOG = 5;
    static constexpr int DEFAULT_EXPIRY_SECS = 60;
    static constexpr int CLIENT_BUFFER_SIZE = 1024;
    static constexpr int CLEANUP_INTERVAL_SECS = 1;
    static constexpr const char *LOG_FILE = "redis.log";
};

#endif // CONFIGS_H_