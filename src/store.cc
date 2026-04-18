#include "../include/store.h"
#include "../include/logger.h"

Store::Store()
{
    startCleanup();
}

Store::~Store()
{
    stopCleanup();
}

Store &Store::getInstance()
{
    static Store instance;
    return instance;
}

void Store::startCleanup()
{
    running = true;
    cleanup_thread = std::thread(&Store::cleanupLoop, this);
}

void Store::stopCleanup()
{
    running = false;
    if (cleanup_thread.joinable())
        cleanup_thread.join();
}

void Store::cleanupLoop()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(Configs::CLEANUP_INTERVAL_SECS));

        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::chrono::steady_clock::now();
        for (auto it = data.begin(); it != data.end();)
        {
            if (now >= it->second.expiry)
            {
                LOG_INFO("Expired key: " + it->first);
                it = data.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

int Store::set(const std::string &key, const std::string &value, int expiry_secs)
{
    std::lock_guard<std::mutex> lock(mtx);
    data[key] = {value, std::chrono::steady_clock::now() + std::chrono::seconds(expiry_secs)};
    return 1;
}

std::string Store::get(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (data.count(key))
    {
        auto &entry = data[key];
        if (std::chrono::steady_clock::now() < entry.expiry)
        {
            return entry.value;
        }
        else
        {
            data.erase(key);
        }
    }
    return "";
}

int Store::del(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (data.erase(key))
        return 1;
    return 0;
}
