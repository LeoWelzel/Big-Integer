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
    BigInt power, coefficient, remainder;

    bigIntInitialise(&power);
    bigIntCopy(input, &coefficient);

    /* Coefficient = input - 1. */
    bigIntSubtract(&coefficient, &one, &coefficient);
    // TODO: replace above with decrement call
    bigIntDivideMod(&coefficient, &two, &holder, &remainder);

    /* while (coefficient % 2 == 0) */
    while (bigIntCompare(&remainder, &zero) == EQUAL)
    {
        rShiftOneBit(&coefficient);
        // TODO: increment call on power.
        bigIntDivideMod(&coefficient, &two, &holder, &remainder);
    }




    PRNG temp;
}
