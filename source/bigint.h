#ifndef BIG_INTEGER_H_INCLUDED
#define BIG_INTEGER_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BASE_TYPE           unsigned int
#define SIGNED_BASE_TYPE    int
#define FULL_BASE_TYPE      0xffffffff
#define DOUBLE_BASE_TYPE    unsigned long long int
#define WORD_SIZE           (int)(sizeof(BASE_TYPE))
#define SPRINTF_FORMAT_STR  "%.08x"
#define SSCANF_FORMAT_STR   "%8x"

/* Use 1024 bit integers for now. */
#define BIGINT_ARR_SIZE     (int)(1024 / (WORD_SIZE * 8))

typedef struct BigInt BigInt;

struct BigInt
{
    BASE_TYPE data[BIGINT_ARR_SIZE];
};

/* Initialisation. */
void bigIntInitialise(BigInt* b);
void bigIntCopy(const BigInt* input, BigInt* output);
void bigIntFromInt(BigInt* b, const DOUBLE_BASE_TYPE i);
void bigIntFromString(BigInt* b, const char* string, const int n);

/* Conversion. */
BASE_TYPE bigIntToInt(BigInt* b);
void bigIntToString(BigInt* b, char* string, const int n);

/* Arithemtic operations. */
void bigIntAdd(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntSubtract(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntMultiply(const BigInt* input1, const BigInt* input2, BigInt* output);

/* Bitwise operations. */
void bigIntLShift(const BigInt* input, BigInt* output, unsigned int numBits);
void bigIntRShift(const BigInt* input, BigInt* output, unsigned int numBits);
void bigIntOr(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntAnd(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntXor(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntComplement(const BigInt* input, BigInt* output);

#ifdef __cplusplus
}
#endif

#endif