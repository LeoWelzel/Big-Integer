#include "bigint.h"

#include <stdlib.h>

void bigIntInitialise(BigInt* b)
{
    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        b->data[i] = 0;
}

void bigIntFromInt(BigInt* b, const BASE_TYPE i)
{
    bigIntInitialise(b);
    b->data[0] = i;
}
