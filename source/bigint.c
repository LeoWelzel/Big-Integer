#include "bigint.h"

void bigIntInitialise(BigInt* b)
{
    assert(b);
    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        b->data[i] = 0;
}

void bigIntFromInt(BigInt* b, const BASE_TYPE i)
{
    assert(b);
    bigIntInitialise(b);
    b->data[0] = i;
}

void bigIntToString(BigInt* b, char* string, const int n)
{
    assert(b);
    assert(string);
    assert(n > 0);
    assert((n % 2) == 0);

    /* Iterate backward over array. */
    int arrayIndex = BIGINT_ARR_SIZE - 1, stringIndex = 0;

    while (arrayIndex >= 0 && (stringIndex + 1) < n)
    {
        sprintf(&string[stringIndex], SPRINTF_FORMAT_STR, b->data[arrayIndex]);
        arrayIndex--;
        stringIndex += 2 * WORD_SIZE;
    }

    /* Count leading zeroes. */
    int firstNonZero = 0;
    while (string[firstNonZero] == '0' && (firstNonZero + 1) < n)
        firstNonZero++;
    
    /* Ensure zero does not return an empty string. */
    if (firstNonZero == stringIndex)
        firstNonZero--;
    
    for (int i = 0; i < n - firstNonZero; i++)
        string[i] = string[i + firstNonZero];
    
    /* Null terminate string. */
    string[n - firstNonZero] = 0;
}
