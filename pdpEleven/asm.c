
#include "asm.h"
#include "utils.h"
#include "arraylist.h"


/*
 *  Constant
 */

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
    "end: "
};

enum RegType
{
    R0 = 0x00, R1, R2, R3, R4, R5, R6, REG_TOTAL, REG_UNKNOWN
};

static const char* regName[REG_TOTAL] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6"
};

enum CmdType
{
    CMD_MOV = 0x00, CMD_CLR, CMD_BR, CMD_MOVB, CMD_INC,  CMD_HALT, CMD_TOTAL, CMD_UNKNOWN = 0x00
};

static const char* cmdName[CMD_TOTAL] = {
    "mov", "clr", "br", "movb", "inc", "halt"
};

typedef struct Cmd
{
    uint16_t address;
    int cmd;
    char param1[32];
    char param2[32];
} CmdStruct, *CmdStructPtr;

/*
 *  Defenition
 */

void convertProgram(int startCode, const char* text[], int currIndex,  int size);
int parseCommand(const char* str, CmdStructPtr cmd);
void parseMacro(int startCode, const char* text[], int index, int size);

BOOL isRegister(const char* name);
BOOL isMacro(const char* macro);
BOOL isCmdName(const char* cmdName);
BOOL isSynaxKey(const char* synaxKey);
BOOL isReserveName(const char *name);
BOOL isLabel(const char* name);

int convertCmdType(const char* str);

void pushCmdMov(CmdStructPtr cmd, uint16_t* startAddress);
void pushCmdClr(CmdStructPtr cmd, uint16_t* startAddress);
void pushCmdBr(CmdStructPtr cmd, uint16_t* startAddress);
void pushCmdMovb(CmdStructPtr cmd, uint16_t* startAddress);
void pushCmdInc(CmdStructPtr cmd, uint16_t* startAddress);
void pushCmdHalt(CmdStructPtr, uint16_t* startAddress);

typedef void (*FuncConvertCmd)(CmdStructPtr cmd, uint16_t* startAddress);
static const FuncConvertCmd funcConvertCmd[CMD_TOTAL] = {
    &pushCmdMov, &pushCmdClr, &pushCmdBr, &pushCmdMovb, &pushCmdInc, &pushCmdHalt
};

/*
 *  Implementation
 */

int assembly(const char* text[], int size)
{
    uint16_t startCode = 0;
    int i;

    const char* str = NULL;

    for(i = 0; i < size; i++)
    {
        str = text[i];

        if((str[0] == ';'))
        {
            continue;
        }
        else if(strStartWith(str, synaxKey[SKEY_ORIGIN]))
        {
            str = str + strlen(synaxKey[SKEY_ORIGIN]);
            sscanf(str, "%ol", &startCode);
            printf("\nStart code: %0o\n", startCode);
        }
        else if(strStartWith(str, synaxKey[SKEY_START]))
        {
            convertProgram(startCode, text, i+1, size);
        }
    }

    return TRUE;
}

void convertProgram(int startCode, const char* text[], int currIndex,  int size)
{
    int i;

    CmdStruct cmds[256];
    CmdStructPtr cmd = NULL;

    int sizeCmds = 0;
    uint16_t address = startCode;

    for(i = currIndex; i < size; i++)
    {
        cmd = &(cmds[sizeCmds]);

        if(strStartWith(text[i], synaxKey[SKEY_DONE]))
        {
            parseCommand(text[i] + strlen(synaxKey[SKEY_DONE]), cmd);
            funcConvertCmd[cmd->cmd](cmd, &address);

            parseMacro(address, text, i+1, size);
            break;
        }

        parseCommand(text[i], cmd);

        funcConvertCmd[cmd->cmd](cmd, &address);

        sizeCmds++;
    }

    for(i = 0; i < sizeCmds; i++)
    {
        printf("\n\t%o CMD: %s, PARAM1: %s, PARAM2: %s",
               cmds[i].address, cmdName[cmds[i].cmd], cmds[i].param1, cmds[i].param2);
    }

    array_print();
}

void convertAsci(const char* src, uint16_t **dst, uint16_t *dstSize)
{
    const uint16_t startAddr = 02000;

    const char *start = strchr(src, '\"') + 1;
    const char *end = strchr(start, '\"');

    uint16_t len = end - start;
    int flag = 0;

    if(len % 2 == 0)
    {
        len++;
        flag = 1;
    }

    *dst = (uint16_t*)malloc(len * 2 * sizeof(uint16_t));

    int i = 0, j = 0;

    while(start != end)
    {
        (*dst)[i] = startAddr + (i*2);

        (*dst)[i+1] = 0;
        (*dst)[i+1] = *start;

        if(j != len - 1)
            (*dst)[i+1] |= (*(start+1) << 8);
        else
            break;

        i += 2;
        j += 2;
        start += 2;
    }

    if(flag)
    {
        (*dst)[len - 1] = startAddr + ((len - 1)*2);
        (*dst)[len] = 0x00;
    }

    (*dstSize) = len;
}

int getOffsetCmd(const char* str)
{
    if(strCompare(str, "mov"))
    {
        return 4;
    }
    else if(strCompare(str, "clr"))
    {
        return 2;
    }
    else
        return 2;
}

int parseCommand(const char* str, CmdStructPtr cmd)
{
    char cmdName[32] = "";
    char param1[32] = "";
    char param2[32] = "";

    int err = sscanf(str, "%s %[^','], %s",
                     cmdName, param1, param2);

    cmd->cmd = convertCmdType(cmdName);
    strcpy(cmd->param1, param1);
    strcpy(cmd->param2, param2);

    return err;
}

void parseMacro(int startCode, const char* text[], int index, int size)
{
    int i;
    char buffer[128] = "";

    for(i = index; i < size; i++)
    {
        sscanf(text[i], "msga: .string \"%s", buffer);
        printf("%s\n", buffer);
    }
}

BOOL isRegister(const char* name)
{
    int i;
    for(i = 0; i < REG_TOTAL; i++)
    {
        if(strCompare(name, regName[i]))
            return TRUE;
    }

    return FALSE;
}

BOOL isMacro(const char* macro)
{
    return (macro[0] == '#');
}

BOOL isCmdName(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, cmdName[i]))
            return TRUE;
    }

    return FALSE;
}

BOOL isSynaxKey(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, synaxKey[i]))
            return TRUE;
    }

    return FALSE;
}

BOOL isReserveName(const char* name)
{
    return isCmdName(name) ||
           isCmdName(name) ||
           isRegister(name);
}

BOOL isLabel(const char* name)
{
    int poslastCh = strlen(name) - 1;
    return (name[poslastCh] == ':') ? TRUE : FALSE;
}

int convertCmdType(const char* str)
{
    if(strCompare(str, "mov"))
    {
        return CMD_MOV;
    }
    else if(strCompare(str, "clr"))
    {
        return CMD_CLR;
    }
    else if(strCompare(str, "movb"))
    {
        return CMD_MOVB;
    }
    else if(strCompare(str, "inc"))
    {
        return CMD_INC;
    }
    else if(strCompare(str, "br"))
    {
        return CMD_BR;
    }
    else if(strCompare(str, "halt"))
    {
        return  CMD_HALT;
    }
    else
        return 2;
}

int getRegAddr(const char* name)
{
    for(int i = 0; i < REG_TOTAL; i++)
    {
        if(strCompare(name, regName[i]))
            return i;
    }

    return REG_UNKNOWN;
}

#define BUILD_CMD(cmdNumb, param, regNumb) \
    (((cmdNumb & 0xF) << 12) | ((param & 0x3F) << 6) | (regNumb & 0x3F))

void pushCmdMov(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode;
    int addr1;
    int addr2;

    optcode = 0x01;
    addr1   = 0x17;
    addr2   = getRegAddr(cmd->param2);

    array_push(*startAddress);
    array_push(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;

    optcode = 0x00;
    addr1   = 0x00;
    addr2   = 0x00;

    array_push(*startAddress);
    array_push(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;
}

void pushCmdClr(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x28;
    int addr2 = getRegAddr(cmd->param1);

    array_push(*startAddress);
    array_push(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;
}

void pushCmdBr(CmdStructPtr cmd, uint16_t* startAddress)
{
    array_push(*startAddress);
    array_push(BUILD_CMD(0xFFFF, 0xFFFF, 0xFFFF));
    *startAddress += 2;
}

void pushCmdMovb(CmdStructPtr cmd, uint16_t* startAddress)
{
    array_push(*startAddress);
    array_push(BUILD_CMD(0xFFFF, 0xFFFF, 0xFFFF));
    *startAddress += 2;
}

void pushCmdInc(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x2A;
    int addr2 = getRegAddr(cmd->param1);

    array_push(*startAddress);
    array_push(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;
}

void pushCmdHalt(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x00;
    int addr2 = 0x00;

    array_push(*startAddress);
    array_push(BUILD_CMD(optcode, addr1, addr2));
}
