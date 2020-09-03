#include <stdio.h>

#include "bigint.h"

int main()
{
    BigInt i;
    bigIntFromInt(&i, 511);

    const int stringSize = 1024;
    char string[stringSize];

    bigIntToString(&i, string, stringSize);
    printf(string);
    return 0;
}