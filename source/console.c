#include "console.h"

static const int STRING_LENGTH = BIGINT_ARR_SIZE * sizeof(BASE_TYPE) * 2;

void consoleLShift(const char* arg1, const char* arg2)
{
    BigInt input, output;

    int bitShift = 0;
    char result[STRING_LENGTH];

    sscanf(arg2, "%i", &bitShift);
    bigIntFromString(&input, arg1, strlen(arg1));
    bigIntLShift(&input, &output, bitShift);

    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}

void consoleRShift(const char* arg1, const char* arg2)
{
    BigInt input, output;

    int bitShift = 0;
    char result[STRING_LENGTH];

    sscanf(arg2, "%i", &bitShift);
    bigIntFromString(&input, arg1, strlen(arg1));
    bigIntRShift(&input, &output, bitShift);

    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}

void consoleAnd(const char* arg1, const char* arg2)
{
    BigInt input1, input2, output;
    char result[STRING_LENGTH];

    bigIntFromString(&input1, arg1, strlen(arg1));
    bigIntFromString(&input2, arg2, strlen(arg2));
    bigIntAnd(&input1, &input2, &output);

    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}

void consoleOr(const char* arg1, const char* arg2)
{
    BigInt input1, input2, output;
    char result[STRING_LENGTH];

    bigIntFromString(&input1, arg1, strlen(arg1));
    bigIntFromString(&input2, arg2, strlen(arg2));
    bigIntOr(&input1, &input2, &output);

    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}

void consoleXor(const char* arg1, const char* arg2)
{
    BigInt input1, input2, output;
    char result[STRING_LENGTH];

    bigIntFromString(&input1, arg1, strlen(arg1));
    bigIntFromString(&input2, arg2, strlen(arg2));
    bigIntXor(&input1, &input2, &output);

    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}
