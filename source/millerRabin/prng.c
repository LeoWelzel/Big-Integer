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

U64 prngRandInt(PRNG* p, const U64 lower, const U64 higher)
{
    assert(lower < higher);

    return (prngRand64(p) % (higher - lower)) + lower;
}
