#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "debug.h"
#include "echo.h"

Echo::Echo() {};

std::string Echo::parseSingleQuotes(std::string input)
{
    if (input.front() == '\'' && input.back() == '\'')
    {
        input = input.substr(1);
        input.pop_back();
        return input;
    }

    input = this->removeExtraSpaces(input);
    return input;
}

std::string Echo::removeExtraSpaces(std::string input)
{
    std::istringstream iss(input);
    std::string word, result;

    while (iss >> word)
    {
        if (!(result.empty()))
        {
            result += " ";
            debug("result: " + result);
        }
        result += word;
        debug("result: " + result);
    }

    return result;
}

std::string Echo::parseInput(std::string input)
{
    input = input.substr(5);
    input = this->parseSingleQuotes(input);

    return input;
}

void Echo::run(std::string input)
{
    debug("input, Initial stage: " + input);
    std::string parsedInput = this->parseInput(input);
    std::cout << parsedInput << "\n";
}
