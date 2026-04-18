#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <string>

#include "input_parser.h"
#include "store.h"

struct CommandResult
{
    std::string response;
    bool should_close;
};

class CommandHandler
{
public:
    CommandHandler();
    CommandResult execute(const Input &input);

private:
    Store &store;
    CommandResult handleSet(const Input &input);
    CommandResult handleGet(const Input &input);
    CommandResult handleDel(const Input &input);
    CommandResult handlePing(const Input &input);
    CommandResult handleEcho(const Input &input);
    CommandResult handleUnknown(const Input &input);
    CommandResult handleQuit(const Input &input);
};

#endif // COMMAND_HANDLER_H_
