#ifndef XORSHIFT_H_INCLUDED
#define XORSHIFT_H_INCLUDED

#include <assert.h>

typedef struct PRNG PRNG;
typedef unsigned long long int U64;

/* Refer to http://vigna.di.unimi.it/ftp/papers/xorshift.pdf. */
struct PRNG
{
    U64 current;
};

void prngSeed(PRNG* p, const U64 seed);
U64 prngRand64(PRNG* p);
U64 prngRandInt(PRNG* p, const U64 lower, const U64 higher);

#endif