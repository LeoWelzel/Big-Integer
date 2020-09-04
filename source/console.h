#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>

#include "bigint.h"

void consoleLShift(const char* arg1, const char* arg2);
void consoleRShift(const char* arg1, const char* arg2);

void consoleAnd(const char* arg1, const char* arg2);
void consoleOr(const char* arg1, const char* arg2);
void consoleXor(const char* arg1, const char* arg2);

#ifdef __cplusplus
}
#endif

#endif