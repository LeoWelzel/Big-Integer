#ifndef BIG_INTEGER_H_INCLUDED
#define BIG_INTEGER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
    #define BIGINT_FUNC extern "C"
#else
    #define BIGINT_FUNC
#endif

#define BASE_TYPE           int
#define WORD_SIZE           sizeof(BASE_TYPE)
#define SPRINTF_FORMAT_STR  "%.08x"

/* Use 1024 bit integers for now. */
#define BIGINT_ARR_SIZE     1024 / (WORD_SIZE * 8)

typedef struct BigInt BigInt;

struct BigInt
{
    BASE_TYPE data[BIGINT_ARR_SIZE];
};

/* Initialisation. */
BIGINT_FUNC void bigIntInitialise(BigInt* b);
BIGINT_FUNC void bigIntFromInt(BigInt* b, const BASE_TYPE i);

/* Conversion. */
BIGINT_FUNC void bigIntToString(BigInt* b, char* string, const int n);

#endif