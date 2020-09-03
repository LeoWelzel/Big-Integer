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

struct BigInt
{
    BASE_TYPE data[BIGINT_ARR_SIZE];
};

#endif