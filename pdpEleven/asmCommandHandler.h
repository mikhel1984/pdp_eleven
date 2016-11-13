
#ifndef ASMCOMMANDHANDLER_H
#define ASMCOMMANDHANDLER_H

#include "common.h"
#include "asmConstant.h"
#include "utils.h"

enum CmdType
{
    CMD_MOV = 0x00,
    CMD_CLR,
    CMD_BR,
    CMD_MOVB,
    CMD_BEQ,
    CMD_INC,
    CMD_HALT,
    CMD_TOTAL,
    CMD_UNKNOWN
};

static const char* getCommandName(int type)
{
    switch(type)
    {
        case CMD_MOV : return "mov";
        case CMD_CLR : return "clr";
        case CMD_BR  : return "br";
        case CMD_MOVB: return "movb";
        case CMD_BEQ : return "beq";
        case CMD_INC : return "inc";
        case CMD_HALT: return "halt";
        default: return "";
    }
}

enum SyntaxKeyType
{
    SKEY_ORIGIN = 0x00,
    SKEY_START,
    SKEY_DONE,
    SKEY_END,
    SKEY_TOTAL
};

static const char* synaxKey[SKEY_TOTAL] = {
    ".origin",
    "start:",
    "done: ",
    ".end"
};

typedef void (*FuncConvertCmd)(CmdStructPtr cmd);

extern const FuncConvertCmd funcConvertCmd[CMD_TOTAL];

static BOOL isCmdName(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, getCommandName(i)))
            return TRUE;
    }

    return FALSE;
}

static int convertCmdType(const char* str)
{
    if(strCompare(str, "mov")      ) return CMD_MOV;
    else if(strCompare(str, "clr") ) return CMD_CLR;
    else if(strCompare(str, "movb")) return CMD_MOVB;
    else if(strCompare(str, "inc") ) return CMD_INC;
    else if(strCompare(str, "br")  ) return CMD_BR;
    else if(strCompare(str, "halt")) return CMD_HALT;
    else if(strCompare(str, "beq") ) return CMD_BEQ;
    else                             return CMD_UNKNOWN;
}

static BOOL isSynaxKey(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, synaxKey[i]))
            return TRUE;
    }

    return FALSE;
}

#endif // ASMCOMMANDHANDLER_H
