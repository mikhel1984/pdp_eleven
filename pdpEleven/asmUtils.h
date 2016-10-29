
#ifndef ASMUTILS_H
#define ASMUTILS_H

#include "common.h"

extern BOOL isRegister(const char* name);
extern BOOL isMacro(const char* macro);
extern BOOL isCmdName(const char* cmdName);
extern BOOL isSynaxKey(const char* synaxKey);
extern BOOL isReserveName(const char *name);
extern BOOL isLabel(const char* name);

extern int convertCmdType(const char* str);

#endif // ASMUTILS_H
