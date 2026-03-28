#ifndef STORE_H_
#define STORE_H_

#include <unordered_map>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>

class Store
{
public:
    static Store *getInstance();
    static void destroyInstance();

    int set(const std::string &key, const std::string &value, int expiry_mins = 1);
    std::string get(const std::string &key);
    int del(const std::string &key);

private:
    struct Entry
    {
        std::string value;
        std::chrono::steady_clock::time_point expiry;
    };

    static std::mutex mtx;
    std::unordered_map<std::string, Entry> data;
    static Store *instance;

    std::atomic<bool> running{false};
    std::thread cleanup_thread;

    Store();
    ~Store();

    void startCleanup();
    void stopCleanup();
    void cleanupLoop();
};

#endif // STORE_H_