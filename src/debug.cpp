#include <iostream>
#include <string>

#include "debug.h"

bool DEBUG = false;

void debug(std::string str)
{
    if (DEBUG)
    {
        std::cout << "DEBUG: " << str << "\n";
    }
}
