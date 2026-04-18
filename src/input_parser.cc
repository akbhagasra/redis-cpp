#include "../include/input_parser.h"
#include "../include/logger.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

static bool parseBulkString(const std::string &input, size_t &pos, std::string &out)
{
    if (pos >= input.size() || input[pos] != '$')
        return false;
    pos++; // skip '$'

    size_t crlf = input.find("\r\n", pos);
    if (crlf == std::string::npos)
        return false;

    int len = std::stoi(input.substr(pos, crlf - pos));
    pos = crlf + 2; // skip past \r\n

    if (len < 0)
    {
        out = "";
        return true;
    }

    if (pos + static_cast<size_t>(len) > input.size())
        return false;

    out = input.substr(pos, len);
    pos += len;

    // skip trailing \r\n
    if (pos + 1 < input.size() && input[pos] == '\r' && input[pos + 1] == '\n')
        pos += 2;

    return true;
}

static CommandType matchCommand(const std::string &cmd)
{
    if (cmd == "SET")
        return CommandType::SET;
    if (cmd == "GET")
        return CommandType::GET;
    if (cmd == "DEL")
        return CommandType::DEL;
    if (cmd == "PING")
        return CommandType::PING;
    if (cmd == "QUIT")
        return CommandType::QUIT;
    if (cmd == "ECHO")
        return CommandType::ECHO;
    return CommandType::UNKNOWN;
}

Input InputParser::parse(const std::string &raw)
{
    Input result{CommandType::UNKNOWN, {}};
    try
    {
        size_t pos = 1; // skip '*'
        size_t crlf = raw.find("\r\n", pos);
        if (crlf == std::string::npos)
            return result;

        int argc = std::stoi(raw.substr(pos, crlf - pos));
        pos = crlf + 2;

        if (argc <= 0)
            return result;

        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
        {
            std::string arg;
            if (!parseBulkString(raw, pos, arg))
                return result;
            args.push_back(std::move(arg));
        }

        std::string cmd = args[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        LOG_DEBUG("RESP Command: <" + cmd + ">");

        result.command = matchCommand(cmd);
        result.args.assign(args.begin() + 1, args.end());
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Failed to parse RESP input: " + std::string(e.what()));
    }

    return result;
}
