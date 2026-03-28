#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

#include "store.h"

class SocketManager
{
public:
    SocketManager();
    ~SocketManager();
    void startServer();

private:
    Store *store;
    void handleClient(int client_fd);
};

#endif // SOCKET_MANAGER_H_