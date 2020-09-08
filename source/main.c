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
        *input3 = malloc(sizeof(BigInt)),
        *output1 = malloc(sizeof(BigInt)),
        *output2 = malloc(sizeof(BigInt));
    bigIntInitialise(input1);
    bigIntInitialise(input2);
    bigIntInitialise(output1);
    bigIntInitialise(output2);

    if (argc <= 1)
    {
        /* Any non-console tests can go here. */
        bigIntFromInt(input1, 9);
        bigIntFromInt(input2, 2);
        bigIntFromInt(input3, 4);

        bigIntModularExponent(input1, input2, input3, output1);
        bigIntToString(output1, result, STRING_LENGTH);

        printf("{%s}\n", result);
        return 0;
    }
    else if (argc >= 3)
    {
        bigIntFromString(input1, argv[2], strlen(argv[2]));
        if (argc == 5)
        {
            if (!strncmp(argv[1], "modexp", 6))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntFromString(input3, argv[4], strlen(argv[4]));
                bigIntModularExponent(input1, input2, input3, output1);
            }
        }
        else if (argc == 4)
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
                bigIntDivide(input1, input2, output1);
            }
            else if (!strncmp(argv[1], "mod", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntMod(input1, input2, output1);
            }
            else UNRECOGNISED_COMMAND;
        }
        else if (argc == 3)
        {
            if (!strncmp(argv[1], "comp", 4))
            {
                bigIntComplement(input1, output1);
            }
            else if (!strncmp(argv[1], "assign", 6))
            {
                bigIntCopy(input1, output1);
            }
            else if (!strncmp(argv[1], "inc", 3))
            {
                bigIntIncrement(input1);
                bigIntCopy(input1, output1);
            }
            else if (!strncmp(argv[1], "dec", 3))
            {
                bigIntDecrement(input1);
                bigIntCopy(input1, output1);
            }
            else if (!strncmp(argv[1], "even", 4))
            {
                bigIntFromInt(output1, bigIntIsEven(input1));
            }
            else UNRECOGNISED_COMMAND;
        }
        else UNRECOGNISED_COMMAND;
        
        bigIntToString(output1, result, STRING_LENGTH);
    }

    printf(result);
    
    return 0;
}