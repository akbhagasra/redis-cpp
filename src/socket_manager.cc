#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>

#include "../include/socket_manager.h"
#include "../include/input_parser.h"
#include "../include/command_handler.h"
#include "../include/logger.h"
#include "../include/configs.h"

void SocketManager::startServer()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        LOG_ERROR("Failed to create socket");
        return;
    }
    LOG_INFO("Socket created successfully");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;            // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;    // Listen on all interfaces
    server_addr.sin_port = htons(Configs::PORT); // Port number (convert to network byte order)

    // 2. Bind the socket to an address and port
    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        LOG_ERROR("Error binding socket");
        return;
    }
    LOG_INFO("Socket bound to port " + std::to_string(Configs::PORT));

    // 3. Listen for incoming connections
    if (listen(server_fd, Configs::BACKLOG) < 0)
    {
        LOG_ERROR("Error listening on socket");
        return;
    }
    LOG_INFO("Listening for incoming connections...");

    // 4. Accept incoming connections
    while (true)
    {
        LOG_DEBUG("Waiting for a connection...");
        int newsockfd = accept(server_fd, nullptr, nullptr);
        if (newsockfd < 0)
        {
            LOG_ERROR("Error accepting connection");
            return;
        }
        LOG_INFO("Connection accepted :: " + std::to_string(newsockfd));
        std::thread(&SocketManager::handleClient, this, newsockfd).detach();
    }
    close(server_fd);
}

void SocketManager::handleClient(int client_fd)
{
    CommandHandler handler;
    char buf[Configs::CLIENT_BUFFER_SIZE];
    while (true)
    {
        ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
        if (n <= 0)
            break;
        buf[n] = '\0';

        Input input = InputParser::parse(buf);
        CommandResult result = handler.execute(input);
        write(client_fd, result.response.c_str(), result.response.size());
        if (result.should_close)
        {
            close(client_fd);
            return;
        }
    }
    LOG_INFO("Closing connection with client:: " + std::to_string(client_fd));
    close(client_fd);
}
