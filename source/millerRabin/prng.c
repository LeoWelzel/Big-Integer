#include "prng.h"

void prngSeed(PRNG* p, const U64 seed)
{
    assert(p);

    p->current = seed;
}

U64 prngRand64(PRNG* p)
{
    assert(p);

    p->current ^= p->current >> 12,
    p->current ^= p->current << 25,
    p->current ^= p->current >> 27;

    return p->current * 2685821657736338717LL;
}

int prngRandInt(PRNG* p, const int lower, const int higher)
{
    assert(lower < higher);

    return (int)(prngRand64(p) % (higher - lower)) + lower;
}
