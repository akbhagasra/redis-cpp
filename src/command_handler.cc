#include "../include/command_handler.h"
#include "../include/configs.h"
#include <algorithm>

CommandHandler::CommandHandler() : store(Store::getInstance()) {}

CommandResult CommandHandler::execute(const Input &input)
{
    switch (input.command)
    {
    case CommandType::SET:
        return handleSet(input);
    case CommandType::GET:
        return handleGet(input);
    case CommandType::DEL:
        return handleDel(input);
    case CommandType::PING:
        return handlePing(input);
    case CommandType::ECHO:
        return handleEcho(input);
    case CommandType::QUIT:
        return handleQuit(input);
    case CommandType::UNKNOWN:
    default:
        return handleUnknown(input);
    }
}

CommandResult CommandHandler::handleSet(const Input &input)
{
    if (input.args.size() < 2)
        return {"-ERR wrong number of arguments for 'SET'\r\n", false};

    const std::string &key = input.args[0];
    const std::string &value = input.args[1];
    int expiry = Configs::DEFAULT_EXPIRY_SECS;

    for (size_t i = 2; i + 1 < input.args.size(); i += 2)
    {
        std::string opt = input.args[i];
        std::transform(opt.begin(), opt.end(), opt.begin(), ::toupper);
        if (opt == "EX")
            expiry = std::stoi(input.args[i + 1]);
        else if (opt == "PX")
            expiry = std::max(1, std::stoi(input.args[i + 1]) / 1000);
    }

    store.set(key, value, expiry);
    return {"+OK\r\n", false};
}

CommandResult CommandHandler::handleGet(const Input &input)
{
    if (input.args.empty())
        return {"-ERR wrong number of arguments for 'GET'\r\n", false};

    std::string value = store.get(input.args[0]);
    if (!value.empty())
        return {"+" + value + "\r\n", false};
    else
        return {"$-1\r\n", false};
}

CommandResult CommandHandler::handleDel(const Input &input)
{
    if (input.args.empty())
        return {"-ERR wrong number of arguments for 'DEL'\r\n", false};

    int count = 0;
    for (const auto &key : input.args)
        count += store.del(key);
    return {":" + std::to_string(count) + "\r\n", false};
}

CommandResult CommandHandler::handlePing(const Input &input)
{
    return {"+PONG\r\n", false};
}

CommandResult CommandHandler::handleEcho(const Input &input)
{
    if (input.args.empty())
        return {"-ERR wrong number of arguments for 'ECHO'\r\n", false};
    return {"+" + input.args[0] + "\r\n", false};
}

CommandResult CommandHandler::handleUnknown(const Input &input)
{
    return {"-ERR unknown command\r\n", false};
}

CommandResult CommandHandler::handleQuit(const Input &input)
{
    return {"+EXITING\r\n", true};
}
