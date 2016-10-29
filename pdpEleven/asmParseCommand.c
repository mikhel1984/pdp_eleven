
#include "asmparsecommand.h"

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
