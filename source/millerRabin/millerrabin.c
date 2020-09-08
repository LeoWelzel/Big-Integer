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
    
    /* input = coefficient * 2 ^ power + 1 */
    BigInt power, coefficient, inputDecremented, attempt, remainder, counter, holder, tempForComparison;
    PRNG prng;
    unsigned long long int upperBound;

    bigIntInitialise(&power);
    bigIntInitialise(&remainder);

    bigIntCopy(input, &coefficient);
    bigIntDecrement(&coefficient);

    bigIntCopy(input, &inputDecremented);
    bigIntDecrement(&inputDecremented);

    while (bigIntIsEven(&coefficient))
    {
        rShiftOneBit(&coefficient);
        bigIntIncrement(&power);
    }

    bigIntFromInt(&tempForComparison, 0xffffffffffffffffull);

    if (bigIntCompare(input, &tempForComparison) == GREATER_THAN)
        upperBound = 0xffffffffffffffffull;
    else
        upperBound = bigIntToDoubleBase(input);

    int continueOuter;

    for (int i = 0; i < iterations; i++)
    {
        continueOuter = 0;
        // TODO: get upper bound on this.
        bigIntFromInt(&attempt, prngRandInt(&prng, 2, upperBound));

        bigIntModularExponent(&attempt, &coefficient, input, &remainder);

        if (bigIntCompare(&remainder, &one) == EQUAL || bigIntCompare(&remainder, &inputDecremented))
            continue;

        bigIntInitialise(&counter);
    
        while (bigIntCompare(&counter, &power) != EQUAL)
        {
            /* remainder = (remainder ^ 2) % input */
            bigIntInitialise(&holder);
            bigIntMultiply(&remainder, &remainder, &holder);
            bigIntMod(&holder, &input, &remainder);

            /* if (remainder == input - 1) */
            if (bigIntCompare(&remainder, &inputDecremented) == EQUAL)
            {
                continueOuter = 1;
                break;
            }
            // TODO: continue from here
        }
    }
}
