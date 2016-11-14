
#ifndef ASMCOMMANDHANDLER_H
#define ASMCOMMANDHANDLER_H

#include "common.h"
#include "asmConstant.h"

typedef void (*FuncConvertCmd)(CmdStructPtr cmd);

extern const FuncConvertCmd funcConvertCmd[CMD_TOTAL];

const char* getCommandName(int type);
BOOL isCmdName(const char* name);
int convertCmdType(const char* str);
BOOL isSynaxKey(const char* name);

#endif // ASMCOMMANDHANDLER_H
