#include "../include/input_parser.h"
#include "../include/logger.h"

Input InputParser::parse(const std::string &raw)
{
    Input result{CommandType::UNKNOWN, "", ""};
    // Strip trailing \r\n or \n (sent by nc/telnet when you press Enter)
    std::string input = raw;
    while (!input.empty() && (input.back() == '\n' || input.back() == '\r'))
        input.pop_back();

    size_t pos = input.find(' ');
    std::string cmd = (pos == std::string::npos) ? input : input.substr(0, pos);
    LOG_DEBUG("Command: <" + cmd + ">");
    if (cmd == "SET")
        result.command = CommandType::SET;
    else if (cmd == "GET")
        result.command = CommandType::GET;
    else if (cmd == "DEL")
        result.command = CommandType::DEL;
    else if (cmd == "PING")
        result.command = CommandType::PING;
    else if (cmd == "QUIT")
        result.command = CommandType::QUIT;
    else
        return result;

    if (pos != std::string::npos)
    {
        size_t key_end = input.find(' ', pos + 1);
        result.key = input.substr(pos + 1, key_end - pos - 1);
        if (key_end != std::string::npos)
            result.value = input.substr(key_end + 1);
    }
    return result;
}
