
#include "asmparsecommand.h"
#include "opcodes.h"

#define BUILD_CMD(cmdNumb, param, regNumb) \
    (((cmdNumb & 0xF) << 12) | ((param & 0x3F) << 6) | (regNumb & 0x3F))

#define BUILD_DO(optCode, Mode1, Reg1, Mode2, Reg2) \
    ((optCode) | (Mode1 << 9) | (Reg1 << 6) | (Mode2 << 3) | Reg2 )

void pushCmdMov(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode;
    int addr1;
    int addr2;

    optcode = 0x01;
    addr1   = 0x17;
    addr2   = getRegAddr(cmd->param2);

    arrayPush(*startAddress);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;

    optcode = 0x00;
    addr1   = 0x00;
    addr2   = 0x00;

    arrayPush(*startAddress);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));
    if(isMacro(cmd->param1))
        dictAdd(macros, cmd->param1+1, arrayCurrIndex());

    *startAddress += 2;
}

void pushCmdBeq(CmdStructPtr cmd, uint16_t* startAddress)
{
    arrayPush(*startAddress);
    arrayPush(opcodes[OP_BEQ].base);

    *startAddress += 2;

    dictAdd(macros, cmd->param1, arrayCurrIndex());
}

void pushCmdClr(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x28;
    int addr2 = getRegAddr(cmd->param1);

    arrayPush(*startAddress);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;
}

void pushCmdBr(CmdStructPtr cmd, uint16_t* startAddress)
{
    int val = opcodes[OP_BR].base;
    int addr = dictFind(macros, cmd->param1, -1);
    uint8_t offset = 0;

    if(addr != -1)
    {
        offset = ((addr - *startAddress) / 2)-1;
        val |= offset;
    }

    arrayPush(*startAddress);
    arrayPush(val);

    *startAddress += 2;
}

void pushCmdMovb(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = opcodes[OP_MOVB].base;
    int mode = 2;
    int reg1 = getRegAddr("r1");//getRegAddr(cmd->param1);
    int reg2 = getRegAddr("r2");//getRegAddr(cmd->param2);
    uint16_t param = BUILD_DO(optcode, mode, reg1, mode, reg2);

    arrayPush(*startAddress);
    arrayPush(BUILD_DO(optcode, mode, reg1, mode, reg2));
    *startAddress += 2;
}

void pushCmdInc(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x2A;
    int addr2 = getRegAddr(cmd->param1);

    arrayPush(*startAddress);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));

    *startAddress += 2;
}

void pushCmdHalt(CmdStructPtr cmd, uint16_t* startAddress)
{
    int optcode = 0x00;
    int addr1 = 0x00;
    int addr2 = 0x00;
    int value = dictFind(macros, "done", -1);

    int offset = 0;
    int oldVal = 0;

    if(value != -1)
    {
        offset = ((*startAddress - arrayGetValue(value - 1)) / 2) - 1;
        oldVal = arrayGetValue(value);
        oldVal |= offset;

        arraySetValue(value, oldVal);
    }


    arrayPush(*startAddress);
    arrayPush(BUILD_CMD(optcode, addr1, addr2));
}
