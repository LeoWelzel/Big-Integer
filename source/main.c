#include <stdio.h>
#include <string.h>

#include "console.h"

int main(int argc, const char** argv)
{
    if (argc <= 1) return 0;

    if (!strncmp(argv[1], "lshift", 6))
    {
        if (argc < 4) return 0;
        consoleLShift(argv[2], argv[3]);
    }
    else if (!strncmp(argv[1], "rshift", 6))
    {
        if (argc < 4) return 0;
        consoleRShift(argv[2], argv[3]);
    }


    return 0;
}