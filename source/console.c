#include "console.h"

static const int STRING_LENGTH = BIGINT_ARR_SIZE * sizeof(BASE_TYPE) * 2;

void consoleLShift(const char* arg1, const char* arg2)
{
    BigInt input, output;

    int bitShift = 0;

    sscanf(arg2, "%i", &bitShift);
    bigIntFromString(&input, arg1, strlen(arg1));
    bigIntLShift(&input, &output, bitShift);

    char result[STRING_LENGTH];
    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}

void consoleRShift(const char* arg1, const char* arg2)
{
    BigInt input, output;

    int bitShift = 0;

    sscanf(arg2, "%i", &bitShift);
    bigIntFromString(&input, arg1, strlen(arg1));
    bigIntRShift(&input, &output, bitShift);

    char result[STRING_LENGTH];
    bigIntToString(&output, result, STRING_LENGTH);
    printf(result);
}
