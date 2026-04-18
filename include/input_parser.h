#ifndef INPUT_PARSER_H_
#define INPUT_PARSER_H_

#include <string>
#include <vector>

#include "enums.h"

struct Input
{
    CommandType command;
    std::vector<std::string> args;
};

class InputParser
{
public:
    static Input parse(const std::string &raw);
};

#endif // INPUT_PARSER_H_