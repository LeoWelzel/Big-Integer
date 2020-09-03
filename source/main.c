#include <stdio.h>

#include "bigint.h"

int main()
{
    BigInt i;
    bigIntInitialise(&i);

    const int stringSize = 1024;
    char string[stringSize];

    bigIntToString(&i, string, stringSize);
    printf(string);
    return 0;
}