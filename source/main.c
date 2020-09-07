#include <stdio.h>
#include <string.h>

#include "bigint.h"

void printBitsULL(unsigned long long x)
{
    for (int i = 63; i >= 0; i--)
        if ((1ULL << i) & x) printf("1");
        else printf("0");
    printf("\n");
}

void printBitsBigInt(BigInt* b)
{
    for (int i = 0; i < 2; i++)
        for (int j = 31; j >= 0; j--)
            if (b->data[i] & (1ULL << j)) printf("1");
            else printf("0");
    printf("\n");
}

int main(int argc, const char** argv)
{
    #define UNRECOGNISED_COMMAND {printf("Unrecognised command entered."); return 0;}

    static const int STRING_LENGTH = BIGINT_ARR_SIZE * sizeof(BASE_TYPE) * 2;
    char result[STRING_LENGTH];
 
    int bitShift;
    BigInt* input1 = malloc(sizeof(BigInt)),
        *input2 = malloc(sizeof(BigInt)),
        *output1 = malloc(sizeof(BigInt)),
        *output2 = malloc(sizeof(BigInt));
    bigIntInitialise(input1);
    bigIntInitialise(input2);
    bigIntInitialise(output1);
    bigIntInitialise(output2);

    if (argc <= 1)
    {
        /* Any non-console tests can go here. */
        bigIntFromInt(input1, 12);
        bigIntFromInt(input2, 4);
        bigIntDivideMod(input1, input2, output1, output2);
        char result[STRING_LENGTH];
        bigIntToString(output1, result, STRING_LENGTH);
        printf("Quotient: {%s}. ", result);
        bigIntToString(output2, result, STRING_LENGTH);
        printf("Remainder: {%s}.\n", result);
        return 0;
    }
    else if (argc >= 3)
    {
        bigIntFromString(input1, argv[2], strlen(argv[2]));
        if (argc == 4)
        {
            if (!strncmp(argv[1], "lshift", 6))
            {
                sscanf(argv[3], "%i", &bitShift);
                bigIntLShift(input1, output1, bitShift);
            }
            else if (!strncmp(argv[1], "rshift", 6))
            {
                sscanf(argv[3], "%i", &bitShift);
                bigIntRShift(input1, output1, bitShift);
            }
            else if (!strncmp(argv[1], "or", 2))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntOr(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "and", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntAnd(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "xor", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntXor(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "add", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntAdd(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "subtract", 8))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntSubtract(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "multiply", 8))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntMultiply(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "divide", 6))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntDivideMod(input1, input2, output2, output1);
                bigIntToString(output2, result, STRING_LENGTH);
                printf("%s\n", result);
            }
            else UNRECOGNISED_COMMAND;
        }
        else if (argc == 3)
        {
            if (!strncmp(argv[1], "comp", 4))
            {
                bigIntComplement(input1, output1);
            }
            if (!strncmp(argv[1], "assign", 6))
            {
                bigIntCopy(input1, output1);
            }
            else UNRECOGNISED_COMMAND;
        }
        else UNRECOGNISED_COMMAND;
        
        bigIntToString(output1, result, STRING_LENGTH);
    }

    printf(result);
    
    return 0;
}