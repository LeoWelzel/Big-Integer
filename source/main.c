#include <stdio.h>
#include <string.h>

#include "millerRabin/millerrabin.h"

int main(int argc, const char** argv)
{
    #define UNRECOGNISED_COMMAND {printf("Unrecognised command entered."); return 0;}

    const int STRING_LENGTH = BIGINT_ARR_SIZE * sizeof(BASE_TYPE) * 2;
    char result[STRING_LENGTH];

    BigInt* input1 = malloc(sizeof(BigInt)), *input2, *input3,
        *output1 = malloc(sizeof(BigInt));

    bigIntInitialise(input1);
    bigIntInitialise(output1);

    millerInitialiseVars();

    if (argc >= 3)
    {
        input2 = malloc(sizeof(BigInt));
        bigIntFromString(input1, argv[2], strlen(argv[2]));
        if (argc == 5)
        {
            if (!strncmp(argv[1], "modexp", 6))
            {
                input3 = malloc(sizeof(BigInt));
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntFromString(input3, argv[4], strlen(argv[4]));
                bigIntModularExponent(input1, input2, input3, output1);
            }
        }
        else if (argc == 4)
        {
            int bitShift;
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
            else
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));

                if (!strncmp(argv[1], "or", 2))
                    bigIntOr(input1, input2, output1);

                else if (!strncmp(argv[1], "and", 3))
                    bigIntAnd(input1, input2, output1);

                else if (!strncmp(argv[1], "xor", 3))
                    bigIntXor(input1, input2, output1);

                else if (!strncmp(argv[1], "add", 3))
                    bigIntAdd(input1, input2, output1);

                else if (!strncmp(argv[1], "subtract", 8))
                    bigIntSubtract(input1, input2, output1);

                else if (!strncmp(argv[1], "multiply", 8))
                    bigIntMultiply(input1, input2, output1);

                else if (!strncmp(argv[1], "divide", 6))
                    bigIntDivide(input1, input2, output1);

                else if (!strncmp(argv[1], "mod", 3))
                    bigIntMod(input1, input2, output1);

                else UNRECOGNISED_COMMAND;
            }
        }
        else if (argc == 3)
        {
            if (!strncmp(argv[1], "comp", 4))
                bigIntComplement(input1, output1);

            else if (!strncmp(argv[1], "assign", 6))
                bigIntCopy(input1, output1);

            else if (!strncmp(argv[1], "even", 4))
                bigIntFromInt(output1, bigIntIsEven(input1));

            else if (!strncmp(argv[1], "prime", 5))
                bigIntFromInt(output1, millerRabin(input1, 25));

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
            else UNRECOGNISED_COMMAND;
        }
        else UNRECOGNISED_COMMAND;

        bigIntToString(output1, result, STRING_LENGTH);
    }
    #undef UNRECOGNISED_COMMAND

    printf(result);

    return 0;
}