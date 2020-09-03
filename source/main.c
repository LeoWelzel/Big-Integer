#include <stdio.h>
#include <string.h>

#include "bigint.h"

int main()
{
    BigInt i;
    const int stringSize = 256;

    char inputString[8];
    strcpy(inputString, "3829475");

    bigIntFromString(&i, inputString, 8);
    char string[stringSize];

    bigIntToString(&i, string, stringSize);
    printf(string);
    return 0;
}