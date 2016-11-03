
#ifndef ASMPARSECOMMAND_H
#define ASMPARSECOMMAND_H

#include "common.h"
#include "asmConstant.h"

extern void pushCmdMov(CmdStructPtr cmd, uint16_t* startAddress);
extern void pushCmdClr(CmdStructPtr cmd, uint16_t* startAddress);
extern void pushCmdBr(CmdStructPtr cmd, uint16_t* startAddress);
extern void pushCmdMovb(CmdStructPtr cmd, uint16_t* startAddress);
extern void pushCmdInc(CmdStructPtr cmd, uint16_t* startAddress);
extern void pushCmdHalt(CmdStructPtr, uint16_t* startAddress);

typedef void (*FuncConvertCmd)(CmdStructPtr cmd, uint16_t* startAddress);
static const FuncConvertCmd funcConvertCmd[CMD_TOTAL] = {
    &pushCmdMov, &pushCmdClr, &pushCmdBr, &pushCmdMovb, &pushCmdInc, &pushCmdHalt
};

#endif // ASMPARSECOMMAND_H
