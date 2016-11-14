
#ifndef ASMUTILS_H
#define ASMUTILS_H

#include "common.h"

BOOL isRegister(const char* name);
BOOL isMacro(const char* macro);
BOOL isReserveName(const char *name);
BOOL isLabel(const char* name);
uint8_t getRegAddr(const char* name);

#endif // ASMUTILS_H
