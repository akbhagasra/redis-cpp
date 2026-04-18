#ifndef SOCKET_MANAGER_H_
#define SOCKET_MANAGER_H_

class SocketManager
{
public:
    SocketManager() = default;
    ~SocketManager() = default;
    void startServer();

private:
    void handleClient(int client_fd);
};

#endif // SOCKET_MANAGER_H_