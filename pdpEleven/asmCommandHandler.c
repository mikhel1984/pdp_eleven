
#include "asmCommandHandler.h"
#include "asmUtils.h"
#include "arraylist.h"
#include "opcodes.h"
#include "utils.h"

#define BUILD_CMD(cmdNumb, param, regNumb) \
    (((cmdNumb & 0xF) << 12) | ((param & 0x3F) << 6) | (regNumb & 0x3F))

#define BUILD_DO(optCode, Mode1, Reg1, Mode2, Reg2) \
    ((optCode) | (Mode1 << 9) | (Reg1 << 6) | (Mode2 << 3) | Reg2 )

/*
 *  Definition function
 */

void processCmdMov(CmdStructPtr cmd);
void processCmdClr(CmdStructPtr cmd);
void processCmdBr(CmdStructPtr cmd);
void processCmdMovb(CmdStructPtr cmd);
void processCmdBeq(CmdStructPtr cmd);
void processCmdInc(CmdStructPtr cmd);
void processCmdHalt(CmdStructPtr);

const FuncConvertCmd funcConvertCmd[CMD_TOTAL] = {
    &processCmdMov,
    &processCmdClr,
    &processCmdBr,
    &processCmdMovb,
    &processCmdBeq,
    &processCmdInc,
    &processCmdHalt
};


void afterBeq(uint16_t address);
uint8_t calcOffsetForBr(uint16_t address, const char* name);

/*
 *  Implementation function
 */

/// Public function

const char* getCommandName(int type)
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

BOOL isCmdName(const char* name)
{
    for(int i = 0; i < CMD_TOTAL; i++)
    {
        if(strCompare(name, getCommandName(i)))
            return TRUE;
    }

    return FALSE;
}

int convertCmdType(const char* str)
{
    if(strCompare(str, "mov")      ) return CMD_MOV;
    else if(strCompare(str, "clr") ) return CMD_CLR;
    else if(strCompare(str, "movb")) return CMD_MOVB;
    else if(strCompare(str, "inc") ) return CMD_INC;
    else if(strCompare(str, "br")  ) return CMD_BR;
    else if(strCompare(str, "done:")) return CMD_HALT;
    else if(strCompare(str, "beq") ) return CMD_BEQ;
    else                             return CMD_UNKNOWN;
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

/// Private function

void processCmdMov(CmdStructPtr cmd)
{
    uint32_t optcode = opcodes[OP_MOV].base;
    uint32_t addr1   = 0x17;
    uint32_t addr2   = getRegAddr(cmd->param2);
    uint32_t val = BUILD_DO(optcode, 2, 0x7, 0, addr2);
    arrayPush(cmd->address);

    arrayPush(val);

    cmd->address += 2;

    arrayPush(cmd->address);
    arrayPush(0x00);

    if(isMacro(cmd->param1))
        dictAdd(macros, cmd->param1+1, arrayCurrIndex());

    cmd->address += 2;
}

void processCmdBeq(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    arrayPush(opcodes[OP_BEQ].base);

    cmd->address += 2;
    dictAdd(macros, cmd->param1, arrayCurrIndex());
}

void processCmdClr(CmdStructPtr cmd)
{
    int optcode = opcodes[OP_CLR].base;
    int addr1 = 0x28;
    int addr2 = getRegAddr(cmd->param1);

    arrayPush(cmd->address);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));

    cmd->address += 2;
}

void processCmdBr(CmdStructPtr cmd)
{
    uint16_t val = opcodes[OP_BR].base;
    val |= calcOffsetForBr(cmd->address, cmd->param1);

    arrayPush(cmd->address);
    arrayPush(val);

    cmd->address += 2;
}

void processCmdMovb(CmdStructPtr cmd)
{
    int optcode = opcodes[OP_MOVB].base;
    int mode    = 2; // TODO automic
    int reg1    = getRegAddr("r1"); // TODO getRegAddr(cmd->param1);
    int reg2    = getRegAddr("r2"); // TODO getRegAddr(cmd->param2);

    arrayPush(cmd->address);
    arrayPush(BUILD_DO(optcode, mode, reg1, mode, reg2));
    cmd->address += 2;
}

void processCmdInc(CmdStructPtr cmd)
{
    int optcode = opcodes[OP_INC].base;
    int addr1   = 0x2A;
    int addr2   = getRegAddr(cmd->param1);

    arrayPush(cmd->address);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));

    cmd->address += 2;
}

void processCmdHalt(CmdStructPtr cmd)
{
    afterBeq(cmd->address);

    arrayPush(cmd->address);
    arrayPush(opcodes[OP_HALT].base);

    cmd->address += 2;
}

uint8_t calcOffsetForBr(uint16_t address, const char* name)
{
    int addr = dictFind(macros, name, -1);

    return (addr != -1)
            ? ((addr - address) / 2)-1
            : 0;
}

void afterBeq(uint16_t address)
{
    int oldVal = 0;
    int offset = 0;

    int index = dictFind(macros, "done", -1);
    if(index == -1)
        return;

    index -= 1; // set index to address
    oldVal = arrayGetValue(index);
    offset = ((address - arrayGetValue(index)) / 2) - 1;

    oldVal = arrayGetValue(index+1);
    oldVal |= offset;

    arraySetValue(index+1, oldVal);
}
