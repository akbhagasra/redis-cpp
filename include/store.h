#ifndef STORE_H_
#define STORE_H_

#include <unordered_map>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>

#include "configs.h"

class Store
{
public:
    static Store &getInstance();

    int set(const std::string &key, const std::string &value, int expiry_secs = Configs::DEFAULT_EXPIRY_SECS);
    std::string get(const std::string &key);
    int del(const std::string &key);

    Store(const Store &) = delete;
    Store &operator=(const Store &) = delete;

private:
    struct Entry
    {
        std::string value;
        std::chrono::steady_clock::time_point expiry;
    };

    std::mutex mtx;
    std::unordered_map<std::string, Entry> data;

    std::atomic<bool> running{false};
    std::thread cleanup_thread;

    Store();
    ~Store();

    void startCleanup();
    void stopCleanup();
    void cleanupLoop();
};

#endif // STORE_H_