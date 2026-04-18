#include "../include/socket_manager.h"
#include "../include/logger.h"
#include "../include/configs.h"

int main()
{
    Logger::getInstance().init(Configs::LOG_FILE);
    LOG_INFO("Hello, Redis!");
    SocketManager manager;
    manager.startServer();
    return 0;
}