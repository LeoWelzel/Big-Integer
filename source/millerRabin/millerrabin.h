#ifndef MILLER_RABIN_H_INCLUDED
#define MILLER_RABIN_H_INCLUDED

#include "../bigint.h"

#include "prng.h"

/* Initialises all static variables needed by the Miller Rabin test. */
void millerInitialiseVars();

/* Returns 1 if the input is likely to be prime, and 0 if it is known to be composite. */
int millerRabin(const BigInt* input, const int iterations);

#endif