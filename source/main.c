#include <stdio.h>
#include <string.h>

#include "bigint.h"

int main(int argc, const char** argv)
{
    #define UNRECOGNISED_COMMAND {printf("Unrecognised command entered."); return 0;}

    static const int STRING_LENGTH = BIGINT_ARR_SIZE * sizeof(BASE_TYPE) * 2;
    char result[STRING_LENGTH];
 
    if (argc <= 1)
    {
        /* Any non-console tests can go here. */
        return 0;
    }

    int bitShift;
    BigInt* input1 = malloc(sizeof(BigInt)),
        *input2 = malloc(sizeof(BigInt)),
        *output = malloc(sizeof(BigInt));

    if (argc >= 3)
    {
        bigIntFromString(input1, argv[2], strlen(argv[2]));
        if (argc == 4)
        {
            if (!strncmp(argv[1], "lshift", 6))
            {
                sscanf(argv[3], "%i", &bitShift);
                bigIntLShift(input1, output, bitShift);
            }
            else if (!strncmp(argv[1], "rshift", 6))
            {
                sscanf(argv[3], "%i", &bitShift);
                bigIntRShift(input1, output, bitShift);
            }
            else if (!strncmp(argv[1], "or", 2))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntOr(input1, input2, output);
            }
            else if (!strncmp(argv[1], "and", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntAnd(input1, input2, output);
            }
            else if (!strncmp(argv[1], "xor", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntXor(input1, input2, output);
            }
            else if (!strncmp(argv[1], "add", 3))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntAdd(input1, input2, output);
            }
            else if (!strncmp(argv[1], "subtract", 8))
            {
                bigIntFromString(input2, argv[3], strlen(argv[3]));
                bigIntSubtract(input1, input2, output);
            }
            else UNRECOGNISED_COMMAND;
        }
        else if (argc == 3)
        {
            if (!strncmp(argv[1], "comp", 4))
            {
                bigIntComplement(input1, output);
            }
            if (!strncmp(argv[1], "assign", 6))
            {
                bigIntCopy(input1, output);
            }
            else UNRECOGNISED_COMMAND;
        }
        else UNRECOGNISED_COMMAND;
        
        bigIntToString(output, result, STRING_LENGTH);
    }

    printf(result);
    
    return 0;
}