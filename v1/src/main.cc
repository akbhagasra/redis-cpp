#include "../include/socket_manager.h"
#include "../include/logger.h"

int main()
{
    Logger::getInstance().init("redis.log");
    LOG_INFO("Hello, Redis!");
    SocketManager manager;
    manager.startServer();
    return 0;
}