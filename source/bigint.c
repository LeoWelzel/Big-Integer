#include "bigint.h"

static void lShiftArray(BigInt* b, const int numElements);
static void rShiftArray(BigInt* b, const int numElements);

void bigIntInitialise(BigInt* b)
{
    assert(b);
    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        b->data[i] = 0;
}

void bigIntCopy(const BigInt* input, BigInt* output)
{
    assert(input);
    assert(output);

    memcpy(output->data, input->data, sizeof(input->data));
}

void bigIntFromInt(BigInt* b, const BASE_TYPE i)
{
    assert(b);

    bigIntInitialise(b);
    b->data[0] = i;
}

void bigIntFromString(BigInt* b, const char* string, const int n)
{
    assert(b);
    assert(string);
    assert(n > 0);
    assert((n % 2) == 0);
    /* Ensure the string length is a multiple of 2 * the basic type to allow array population. */
    assert(n % sizeof(BASE_TYPE) * 2 == 0);

    bigIntInitialise(b);

    /* Start here because we are passed a hex string. Each character represents 4 bits. */
    int stringIndex = n - 2 * WORD_SIZE, arrayIndex = 0;
    BASE_TYPE buffer;

    while (stringIndex >= 0)
    {
        buffer = 0;

        sscanf(&string[stringIndex], SSCANF_FORMAT_STR, &buffer);

        b->data[arrayIndex] = buffer;
        arrayIndex++;
        stringIndex -= 2 * WORD_SIZE;
    }
}

BASE_TYPE bigIntToInt(BigInt* b)
{
    assert(b);
    return b->data[0];
}

void bigIntToString(BigInt* b, char* string, const int n)
{
    assert(b);
    assert(string);
    assert(n > 0);
    assert((n % 2) == 0);

    /* Iterate backward over array. */
    int arrayIndex = BIGINT_ARR_SIZE - 1, stringIndex = 0;

    while (arrayIndex >= 0 && (stringIndex + 1) < n)
    {
        sprintf(&string[stringIndex], SPRINTF_FORMAT_STR, b->data[arrayIndex]);
        arrayIndex--;
        stringIndex += 2 * WORD_SIZE;
    }

    /* Count leading zeroes. */
    int firstNonZero = 0;
    while (string[firstNonZero] == '0' && (firstNonZero + 1) < n)
        firstNonZero++;
    
    /* Ensure zero does not return an empty string. */
    if (firstNonZero == BIGINT_ARR_SIZE / 4)
        firstNonZero--;
    
    for (int i = 0; i < n - firstNonZero; i++)
        string[i] = string[i + firstNonZero];
    
    /* Null terminate string. */
    string[n - firstNonZero] = 0;
}

void bigIntLShift(const BigInt* input, BigInt* output, unsigned int numBits)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);
    /* If the number of bits to shift is greater than the number of bits in a word, shift array. */
    const int numWords = numBits / (8 * WORD_SIZE);
    if (numWords)
    {
        lShiftArray(output, numWords);
        numBits -= numWords * (8 * WORD_SIZE);
    }

    /* Shift bits over inside words. */    
    for (int i = BIGINT_ARR_SIZE - 1; i > 0; i--)
        /* Shift word over. */
        output->data[i] = (input->data[i] << numBits)
            |
        /* Include spillover from lower word.. */
        (input->data[i - 1] >> (8 * WORD_SIZE - numBits));
    
    output->data[0] = input->data[0] << numBits;
}

void bigIntRShift(const BigInt* input, BigInt* output, unsigned int numBits)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);
    /* If the number of bits to shift is greater than the number of bits in a word, shift array. */
    const int numWords = numBits / (8 * WORD_SIZE);
    if (numWords)
    {
        rShiftArray(output, numWords);
        numBits -= numWords * (8 * WORD_SIZE);
    }

    /* Shift bits over inside words. */
    for (int i = 0; i < BIGINT_ARR_SIZE - 1; i++)
        output->data[i] = (input->data[i] >> numBits
            |
        /* Include spillover from higher word. */
        (input->data[i + 1] << (8 * WORD_SIZE - numBits)));

    output->data[BIGINT_ARR_SIZE - 1] = input->data[BIGINT_ARR_SIZE - 1] >> numBits;
}

static void lShiftArray(BigInt* b, const int numElements)
{
    assert(b);
    assert(numElements >= 0);
    int i;

    for (i = BIGINT_ARR_SIZE - 1; i >= numElements; i--)
        b->data[i] = b->data[i - numElements];
    for (; i >= 0; i--)
        b->data[i] = 0;
}

static void rShiftArray(BigInt* b, const int numElements)
{
    assert(b);
    assert(numElements >= 0);
    int i;

    for (i = 0; i < (int)(BIGINT_ARR_SIZE - numElements); i++)
        b->data[i] = b->data[i + numElements];
    for (; i < BIGINT_ARR_SIZE; i++)
        b->data[i] = 0;
}
