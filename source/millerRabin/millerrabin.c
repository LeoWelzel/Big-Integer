#include "millerrabin.h"

static BigInt zero, one, two, holder;

void initialiseVars()
{
    bigIntFromInt(&zero, 0);
    bigIntFromInt(&one, 1);
    bigIntFromInt(&two, 2);
}



int millerRabin(const BigInt* input, const int iterations)
{
    int result = bigIntCompare(input, &two);

    if (result == LESS_THAN)
        return 0;
    else if (result == EQUAL)
        return 1;
    
    /* Write input as coefficient * 2 ^ power + 1. */
    BigInt power, coefficient, random;

    bigIntInitialise(&power);
    bigIntCopy(input, &coefficient);

    /* Coefficient = input - 1. */
    bigIntDecrement(&coefficient);

    while (bigIntIsEven(&coefficient))
    {
        rShiftOneBit(&coefficient);
        bigIntIncrement(&power);
    }

    for (int i = 0; i < iterations; i++)
    {

    }


    PRNG temp;
}
