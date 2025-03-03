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

// * overload not needed, we arent printing any ints, uncomment if needed in the future
// void debug(int num)
// {
//     if (DEBUG)
//     {
//         std::cout << "DEBUG: " << num << "\n";
//     }
// }
