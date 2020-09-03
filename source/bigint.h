#ifndef BIG_INTEGER_H_INCLUDED
#define BIG_INTEGER_H_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
    #define BIGINT_FUNC extern "C"
#else
    #define BIGINT_FUNC
#endif

#define BASE_TYPE           int
#define WORD_SIZE           sizeof(BASE_TYPE)

/* Use 1024 bit integers for now. */
#define BIGINT_ARR_SIZE     128 / WORD_SIZE

typedef struct BigInt BigInt;

struct BigInt
{
    BASE_TYPE data[BIGINT_ARR_SIZE];
};

/* Initialisation. */
BIGINT_FUNC void bigIntInitialise(BigInt* b);
BIGINT_FUNC void bigIntFromInt(BigInt* b, const BASE_TYPE i);

/* Conversion. */
BIGINT_FUNC void bigIntToString(BigInt* b);

#endif