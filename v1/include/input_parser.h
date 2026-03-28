#ifndef INPUT_PARSER_H_
#define INPUT_PARSER_H_

#include <string>

#include "enums.h"

struct Input
{
    CommandType command;
    std::string key;
    std::string value;
};

class InputParser
{
public:
    static Input parse(const std::string &raw);
};

#endif // INPUT_PARSER_H_