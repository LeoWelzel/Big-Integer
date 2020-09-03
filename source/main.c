#include <stdio.h>
#include <string.h>

#include "bigint.h"

int main()
{
    BigInt bigInt;
    const int stringSize = 256;

    char inputString[32];
    strcpy(inputString, "1234567890123456789012345678901");

    bigIntFromString(&bigInt, inputString, 32);
    char string[stringSize], string2[stringSize];
    bigIntToString(&bigInt, string, stringSize);
    printf("%s\n", string);

    rShiftArray(&bigInt, 1);
    // int x = 5;
    bigIntToString(&bigInt, string, stringSize);
    printf(string);
    return 0;
}