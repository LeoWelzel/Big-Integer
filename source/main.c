#include <stdio.h>
#include <string.h>

#include "console.h"

int main(int argc, const char** argv)
{
    if (argc <= 1)
    {
        /* Any non-console tests can go here. */
        return 0;
    }

    if (argc >= 4)
    {
        if (!strncmp(argv[1], "lshift", 6))
            consoleLShift(argv[2], argv[3]);

        else if (!strncmp(argv[1], "rshift", 6))
            consoleRShift(argv[2], argv[3]);

        else if (!strncmp(argv[1], "or", 2))
            consoleOr(argv[2], argv[3]);

        else if (!strncmp(argv[1], "and", 3))
            consoleAnd(argv[2], argv[3]);

        else if (!strncmp(argv[1], "xor", 3))
            consoleXor(argv[2], argv[3]);
    }
    
    else if(argc)
    
    return 0;
}