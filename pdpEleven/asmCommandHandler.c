
#include "asmCommandHandler.h"
#include "asmUtils.h"
#include "arraylist.h"
#include "opcodes.h"

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
int8_t calcOffsetForBr(uint16_t address, const char* name);

/*
 *  Implementation function
 */

void processCmdMov(CmdStructPtr cmd)
{
    uint16_t optcode = opcodes[OP_MOV].base;
    uint16_t addr1   = 0x17;
    uint16_t addr2   = getRegAddr(cmd->param2);
    uint16_t val     = BUILD_CMD(optcode, addr1, addr2);

    arrayPush(cmd->address);
    arrayPush(val);

    cmd->address += 2;
    val = 0;

    if(isMacro(cmd->param2))
    {
        arrayPush(cmd->address);
        dictAdd(macros, cmd->param1+1, arrayCurrIndex());
    }

    arrayPush(val);

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
    int val = opcodes[OP_BR].base;
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

int8_t calcOffsetForBr(uint16_t address, const char* name)
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
    offset = ((address - arrayGetValue(index)) / 2) - 1;

    oldVal = arrayGetValue(index);
    oldVal |= offset;

    arraySetValue(index, oldVal);
}
