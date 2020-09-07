#include "bigint.h"

static void lShiftArray(BigInt* b, const int numElements);
static void rShiftArray(BigInt* b, const int numElements);
static void lShiftOneBit(BigInt* b);
static void rShiftOneBit(BigInt* b);

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

void bigIntFromInt(BigInt* b, const DOUBLE_BASE_TYPE i)
{
    assert(b);

    bigIntInitialise(b);

    /* Depends on this specific type setup. */
    b->data[0] = i;
    b->data[1] = i >> 32;
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

    while (stringIndex >= 0)
    {
        sscanf(&string[stringIndex], SSCANF_FORMAT_STR, &b->data[arrayIndex]);
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
    while (string[firstNonZero] == '0')
        firstNonZero++;
    
    /* Ensure zero does not return an empty string. */
    if (string[firstNonZero] == '\0')
        firstNonZero--;
    
    for (int i = 0; i < n - firstNonZero; i++)
        string[i] = string[i + firstNonZero];
    
    /* Null terminate string. */
    string[n - firstNonZero] = 0;
}

void bigIntAdd(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    DOUBLE_BASE_TYPE result;
    BASE_TYPE carry = 0;

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
    {
        result = (DOUBLE_BASE_TYPE)input1->data[i] + input2->data[i] + carry;
        carry = (result > FULL_BASE_TYPE);
        output->data[i] = result & FULL_BASE_TYPE;
    }
}

void bigIntSubtract(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    BASE_TYPE take = 0, result;

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
    {
        result = input1->data[i] - input2->data[i] - take;
        
        /* If the first bit of the result is set, we need to deduct from the next word. */
        if (input1->data[i] < input2->data[i] + take) take = 1;
        else take = 0;

        output->data[i] = (BASE_TYPE)result;
    }
}

void bigIntMultiply(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    DOUBLE_BASE_TYPE result;
    BigInt rowSum, tempProduct;

    /* Use long multiplication. */
    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
    {
        bigIntInitialise(&rowSum);

        for (int j = 0; j < BIGINT_ARR_SIZE; j++)
        {
            if (i + j < BIGINT_ARR_SIZE)
            {
                bigIntInitialise(&tempProduct);
                result = (DOUBLE_BASE_TYPE)input1->data[i] *
                    (DOUBLE_BASE_TYPE)input2->data[j];
    
                bigIntFromInt(&tempProduct, result);
                lShiftArray(&tempProduct, i + j);

                bigIntAdd(&tempProduct, &rowSum, &rowSum);
            }
        }

        bigIntAdd(&rowSum, output, output);
    }
}

void bigIntDivideMod(const BigInt* numerator, const BigInt* divisor, BigInt* quotientOutput,
    BigInt* remainderOutput)
{
    /* Refer to https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
     * for pseudocode and explanation. */

    assert(numerator);
    assert(divisor);
    assert(quotientOutput);
    assert(remainderOutput);

    /* Primitive check for division by zero. */
    assert(divisor->data[0]);

    BigInt quotient, remainder;

    bigIntInitialise(&quotient);
    bigIntInitialise(&remainder);

    /* Number of bits used by numerator. */
    int numBits = 0;

    for (int i = BIGINT_ARR_SIZE - 1; i >= 0; i--)
        if (numerator->data[i])
        {
            const int leadingZeroes = COUNT_LEADING_ZEROES(numerator->data[i]);
            numBits = (i + 1) * (8 * WORD_SIZE) - leadingZeroes;
            break;
        } 

    for (int i = numBits - 1; i >= 0; i--)
    {
        // TODO: lshift remainder by one bit
    }
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
        numBits -= numWords * 8 * WORD_SIZE;
    }

    if (!numBits) return;

    /* Shift bits over inside words. */    
    for (int i = BIGINT_ARR_SIZE - 1; i > 0; i--)
        /* Shift word over. */
        output->data[i] = (output->data[i] << numBits)
            |
        /* Include spillover from lower word. */
        (output->data[i - 1] >> (8 * WORD_SIZE - numBits));
    
    output->data[0] <<= numBits;
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

    if (!numBits) return;

    /* Shift bits over inside words. */
    for (int i = 0; i < (BIGINT_ARR_SIZE - 1); i++)
        output->data[i] = (output->data[i] >> numBits
            |
        /* Include spillover from higher word. */
        (output->data[i + 1] << (8 * WORD_SIZE - numBits)));

    output->data[BIGINT_ARR_SIZE - 1] >>= numBits;
}

void bigIntOr(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        output->data[i] = input1->data[i] | input2->data[i];
}

void bigIntAnd(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        output->data[i] = input1->data[i] & input2->data[i];
}

void bigIntXor(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        output->data[i] = input1->data[i] ^ input2->data[i];
}

void bigIntComplement(const BigInt* input, BigInt* output)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        output->data[i] = ~input->data[i];
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

static void lShiftOneBit(BigInt* b)
{
    assert(b);

    for (int i = BIGINT_ARR_SIZE - 1; i > 0; i--)
        b->data[i] = (b->data[i] << 1) | (b->data[i - 1] >> (WORD_SIZE * 8 - 1));
    b->data[0] <<= 1;
}

static void rShiftOneBit(BigInt* b)
{
    assert(b);

    for (int i = 0; i < BIGINT_ARR_SIZE - 1; i++)
        b->data[i] = (b->data[i] >> 1) | (b->data[i + 1] << ((WORD_SIZE * 8 - 1)));
    b->data[BIGINT_ARR_SIZE - 1] >>= 1;
}