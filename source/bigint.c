#include "bigint.h"

static void lShiftArray(BigInt* b, const int numElements);
static void rShiftArray(BigInt* b, const int numElements);
/* The last bit is bit 0. */
static int nthBitSet(const BigInt* b, const int n);
static void setNthBit(BigInt* b, const int n);

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

BASE_TYPE bigIntToInt(const BigInt* b)
{
    assert(b);
    return b->data[0];
}

DOUBLE_BASE_TYPE bigIntToDoubleBase(const BigInt* b)
{
    assert(b);

    DOUBLE_BASE_TYPE output = b->data[0];
    output |= ((DOUBLE_BASE_TYPE)b->data[1] << BITS_PER_WORD);
    return output;
}

void bigIntToString(const BigInt* b, char* string, const int n)
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
            numBits = (i + 1) * BITS_PER_WORD - leadingZeroes;
            break;
        }

    for (int i = numBits - 1; i >= 0; i--)
    {
        lShiftOneBit(&remainder);
        
        if (nthBitSet(numerator, i))
            remainder.data[0] |= 1;

        if (bigIntCompare(&remainder, divisor) != LESS_THAN)
        {
            bigIntSubtract(&remainder, divisor, &remainder);
            setNthBit(&quotient, i);
        }
    }

    bigIntCopy(&quotient, quotientOutput);
    bigIntCopy(&remainder, remainderOutput);
}

void bigIntDivide(const BigInt* numerator, const BigInt* divisor, BigInt* quotient)
{
    assert(numerator);
    assert(divisor);
    assert(quotient);

    BigInt temp;
    bigIntDivideMod(numerator, divisor, quotient, &temp);
}

void bigIntMod(const BigInt* numerator, const BigInt* divisor, BigInt* remainder)
{
    assert(numerator);
    assert(divisor);
    assert(remainder);

    BigInt temp;
    bigIntDivideMod(numerator, divisor, &temp, remainder);
}

void bigIntIncrement(BigInt* input)
{
    assert(input);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
    {
        input->data[i]++;
        if (input->data[i]) return;
    }
}

void bigIntDecrement(BigInt* input)
{
    assert(input);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
    {
        input->data[i]--;

        if (~input->data[i]) return;
    }
}

int bigIntCompare(const BigInt* left, const BigInt* right)
{
    assert(left);
    assert(right);

    for (int i = BIGINT_ARR_SIZE - 1; i >= 0; i--)
    {
        if (left->data[i] > right->data[i])
            return GREATER_THAN;
        else if (left->data[i] < right->data[i])
            return LESS_THAN;
    }

    return EQUAL;
}

void bigIntLShift(const BigInt* input, BigInt* output, unsigned int numBits)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);

    /* If the number of bits to shift is greater than the number of bits in a word, shift array. */
    const int numWords = numBits / BITS_PER_WORD;
    if (numWords)
    {
        lShiftArray(output, numWords);
        numBits -= numWords * BITS_PER_WORD;
    }

    if (!numBits) return;

    /* Shift bits over inside words. */    
    for (int i = BIGINT_ARR_SIZE - 1; i > 0; i--)
        /* Shift word over. */
        output->data[i] = (output->data[i] << numBits)
            |
        /* Include spillover from lower word. */
        (output->data[i - 1] >> (BITS_PER_WORD - numBits));
    
    output->data[0] <<= numBits;
}

void bigIntRShift(const BigInt* input, BigInt* output, unsigned int numBits)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);

    /* If the number of bits to shift is greater than the number of bits in a word, shift array. */
    const int numWords = numBits / BITS_PER_WORD;
    if (numWords)
    {
        rShiftArray(output, numWords);
        numBits -= numWords * BITS_PER_WORD;
    }

    if (!numBits) return;

    /* Shift bits over inside words. */
    for (int i = 0; i < (BIGINT_ARR_SIZE - 1); i++)
        output->data[i] = (output->data[i] >> numBits
            |
        /* Include spillover from higher word. */
        (output->data[i + 1] << (BITS_PER_WORD - numBits)));

    output->data[BIGINT_ARR_SIZE - 1] >>= numBits;
}

void lShiftOneBit(BigInt* b)
{
    assert(b);

    for (int i = BIGINT_ARR_SIZE - 1; i > 0; i--)
        b->data[i] = (b->data[i] << 1) | (b->data[i - 1] >> (WORD_SIZE * 8 - 1));
    b->data[0] <<= 1;
}

void rShiftOneBit(BigInt* b)
{
    assert(b);

    for (int i = 0; i < BIGINT_ARR_SIZE - 1; i++)
        b->data[i] = (b->data[i] >> 1) | (b->data[i + 1] << ((WORD_SIZE * 8 - 1)));
    b->data[BIGINT_ARR_SIZE - 1] >>= 1;
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

int bigIntIsEven(const BigInt* input)
{
    assert(input);
    return !(input->data[0] % 2);
}

int bigIntIsZero(const BigInt* input)
{
    assert(input);
    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        if (input->data[i]) return 0;
    return 1;
}

void bigIntModularExponent(const BigInt* base, const BigInt* exponent, const BigInt* mod, BigInt* output)
{
    assert(base);
    assert(exponent);
    assert(mod);
    assert(output);

    BigInt holder, baseCopy, exponentCopy;

    bigIntInitialise(&holder);
    bigIntInitialise(&baseCopy);
    bigIntCopy(exponent, &exponentCopy);
    bigIntFromInt(output, 1ull);

    /* baseCopy = base / mod */
    bigIntMod(base, mod, &baseCopy);

    /* if (baseCopy == 0) return 0; */
    if (bigIntIsZero(&baseCopy))
    {
        bigIntInitialise(output);
        return;
    }

    while (!bigIntIsZero(&exponentCopy))
    {
        if (!bigIntIsEven(&exponentCopy))
        {
            /* output = (output * base) % mod; */
            bigIntInitialise(&holder);
            bigIntMultiply(output, &baseCopy, &holder);
            bigIntInitialise(output);
            bigIntMod(&holder, mod, output);
        }

        rShiftOneBit(&exponentCopy);

        /* baseCopy = (baseCopy * baseCopy) % mod; */
        bigIntInitialise(&holder);
        bigIntMultiply(&baseCopy, &baseCopy, &holder);
        bigIntInitialise(&baseCopy);
        bigIntMod(&holder, mod, &baseCopy);
    }
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

static int nthBitSet(const BigInt* b, const int n)
{
    assert(b);
    assert(n >= 0);
    assert(n < TOTAL_BITS);

    return (b->data[n / BITS_PER_WORD] >> (n % BITS_PER_WORD)) & 1;
}

static void setNthBit(BigInt* b, const int n)
{
    assert(b);
    assert(n >= 0);
    assert(n < TOTAL_BITS);

    b->data[n / BITS_PER_WORD] |= (BASE_TYPE)1 << (n % BITS_PER_WORD);
}
