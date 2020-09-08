#ifndef MILLER_RABIN_H_INCLUDED
#define MILLER_RABIN_H_INCLUDED

#include "../bigint.h"

#include "prng.h"

/* Initialises all static variables needed by the Miller Rabin test. */
void initialiseVars();
int millerRabin(const BigInt* input, const int iterations);

#endif