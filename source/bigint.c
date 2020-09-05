#include "bigint.h"

static void lShiftArray(BigInt* b, const int numElements);
static void rShiftArray(BigInt* b, const int numElements);

void bigIntInitialise(BigInt* b)
{
    assert(b);

    for (int i = 0; i < BIGINT_ARR_SIZE; i++)
        b->data[i] = 0;
    
    b->numElements = 0;
}

void bigIntCopy(const BigInt* input, BigInt* output)
{
    assert(input);
    assert(output);

    memcpy(output->data, input->data, sizeof(input->data));
    output->numElements = input->numElements;
}

void bigIntFromInt(BigInt* b, const BASE_TYPE i)
{
    assert(b);

    bigIntInitialise(b);
    b->data[0] = i;

    b->numElements = 1;
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

    b->numElements = arrayIndex;
}

BASE_TYPE bigIntToInt(BigInt* b)
{
    assert(b);
    return b->numElements ? b->data[0] : 0;
}

void bigIntToString(BigInt* b, char* string, const int n)
{
    assert(b);
    assert(string);
    assert(n > 0);
    assert((n % 2) == 0);

    /* Iterate backward over array. */
    int arrayIndex = b->numElements - 1, stringIndex = 0;

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

    bigIntInitialise(output);

    int higher = (input1->numElements > input2->numElements) ? input1->numElements : input2->numElements;

    for (int i = 0; i < higher; i++)
    {
        result = (DOUBLE_BASE_TYPE)input1->data[i] + input2->data[i] + carry;
        carry = (result > FULL_BASE_TYPE);
        output->data[i] = result & FULL_BASE_TYPE;
    }

    if (carry && higher < BIGINT_ARR_SIZE)
    {
        output->data[higher] = carry;
        higher++;
    }
    
    output->numElements = higher;
}

void bigIntSubtract(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    BASE_TYPE take = 0, result;

    bigIntInitialise(output);

    int higher = (input1->numElements > input2->numElements) ? input1->numElements : input2->numElements,
        
        /* The highest index which is not negative. */
        /* We want to avoid producing numbers prefixed by excessive 1s. */
        highestPopulated = 0;

    for (int i = 0; i < higher; i++)
    {
        result = input1->data[i] - input2->data[i] - take;
        
        /* If the first bit of the result is set, we need to deduct from the next word. */
        if (input1->data[i] < input2->data[i] + take) take = 1;
        else take = 0;

        output->data[i] = (BASE_TYPE)result;

        if (~result) highestPopulated = i;
    }

    output->numElements = highestPopulated + 1;
}

void bigIntMultiply(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);



    /* Use long multiplication. */
    bigIntInitialise(output);

    for (int i = 0; i < input1->numElements; i++)
    {
        for (int j = 0; j < input2->numElements; j++)
        {
            
        }
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

    /* Increment numElements if highest word spills over. */
    if (output->data[output->numElements - 1] >> (8 * WORD_SIZE - numBits)
        && output->numElements < BIGINT_ARR_SIZE)
        output->numElements++;

    /* Shift bits over inside words. */    
    for (int i = output->numElements - 1; i > 0; i--)
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
    for (int i = 0; i < output->numElements - 1; i++)
        output->data[i] = (output->data[i] >> numBits
            |
        /* Include spillover from higher word. */
        (output->data[i + 1] << (8 * WORD_SIZE - numBits)));

    output->data[output->numElements - 1] >>= numBits;

    /* Decrement numElements if highest word truncated. */
    if (!output->data[output->numElements - 1] && output->numElements > 0)
        output->numElements--;
}

void bigIntOr(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    bigIntInitialise(output);

    int higher = (input1->numElements > input2->numElements) ? input1->numElements : input2->numElements;
    output->numElements = higher;

    for (int i = 0; i < higher; i++)
        output->data[i] = input1->data[i] | input2->data[i];
}

void bigIntAnd(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    bigIntInitialise(output);

    int higher = (input1->numElements > input2->numElements) ? input1->numElements : input2->numElements;
    output->numElements = higher;

    for (int i = 0; i < higher; i++)
        output->data[i] = input1->data[i] & input2->data[i];
}

void bigIntXor(const BigInt* input1, const BigInt* input2, BigInt* output)
{
    assert(input1);
    assert(input2);
    assert(output);

    bigIntInitialise(output);

    int higher = (input1->numElements > input2->numElements) ? input1->numElements : input2->numElements;
    output->numElements = higher;

    for (int i = 0; i < higher; i++)
        output->data[i] = input1->data[i] ^ input2->data[i];
}

void bigIntComplement(const BigInt* input, BigInt* output)
{
    assert(input);
    assert(output);

    bigIntCopy(input, output);

    for (int i = 0; i < input->numElements; i++)
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
    
    b->numElements += numElements;
    if (b->numElements > BIGINT_ARR_SIZE) b->numElements = BIGINT_ARR_SIZE;
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
    
    b->numElements -= numElements;
    if (b->numElements < 0) b->numElements = 0;
}
