#pragma once

#include <string>
#include <vector>

class Echo
{
private:
    std::string removeExtraSpaces(std::string input);
    std::string parseInput(std::string input);
    std::string parseSingleQuotes(std::string input);

public:
    Echo();
    void run(std::string input);
};
