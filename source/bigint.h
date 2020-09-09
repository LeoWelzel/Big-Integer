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

#define BASE_TYPE               unsigned int
#define SIGNED_BASE_TYPE        int
#define FULL_BASE_TYPE          0xffffffff
#define DOUBLE_BASE_TYPE        unsigned long long int
#define SPRINTF_FORMAT_STR      "%.08x"
#define SSCANF_FORMAT_STR       "%8x"
#define COUNT_LEADING_ZEROES    __builtin_clz

/* Use 1024 bit integers for now. */
#define WORD_SIZE               ((int)(sizeof(BASE_TYPE)))
#define BITS_PER_WORD           (8 * WORD_SIZE)
#define TOTAL_BITS              (1024)
#define BIGINT_ARR_SIZE         ((int)(TOTAL_BITS / BITS_PER_WORD))

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
BASE_TYPE bigIntToInt(const BigInt* b);
DOUBLE_BASE_TYPE bigIntToDoubleBase(const BigInt* b);
void bigIntToString(const BigInt* b, char* string, const int n);

/* Arithemtic operations. */
void bigIntAdd(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntSubtract(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntMultiply(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntDivideMod(const BigInt* numerator, const BigInt* divisor, BigInt* quotient, BigInt* remainder);
void bigIntDivide(const BigInt* numerator, const BigInt* divisor, BigInt* quotient);
void bigIntMod(const BigInt* numerator, const BigInt* divisor, BigInt* remainder);
void bigIntIncrement(BigInt* input);
void bigIntDecrement(BigInt* input);

enum ComparisonOutcome
{
    GREATER_THAN = 1,
    EQUAL = 0,
    LESS_THAN = -GREATER_THAN
};

/* Comparison operations. */
int bigIntCompare(const BigInt* left, const BigInt* right);

/* Bitwise operations. */
void bigIntLShift(const BigInt* input, BigInt* output, unsigned int numBits);
void bigIntRShift(const BigInt* input, BigInt* output, unsigned int numBits);
void lShiftOneBit(BigInt* b);
void rShiftOneBit(BigInt* b);
void bigIntOr(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntAnd(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntXor(const BigInt* input1, const BigInt* input2, BigInt* output);
void bigIntComplement(const BigInt* input, BigInt* output);

/* Miscellaneous functions. */
int bigIntIsEven(const BigInt* input);
int bigIntIsZero(const BigInt* input);
void bigIntModularExponent(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* output);

#ifdef __cplusplus
}
#endif

#endif