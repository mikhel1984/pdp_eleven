
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
void processCmdBne(CmdStructPtr cmd);
void processCmdInc(CmdStructPtr cmd);
void processCmdDec(CmdStructPtr cmd);
void processCmdHalt(CmdStructPtr cmd);
void processCmdRti(CmdStructPtr cmd);
void processCmdJmp(CmdStructPtr cmd);
void processCmdMul(CmdStructPtr cmd);
void processCmdAdd(CmdStructPtr cmd);
void processCmdNop(CmdStructPtr cmd);

const FuncConvertCmd funcConvertCmd[CMD_TOTAL] = {
    &processCmdMov,
    &processCmdClr,
    &processCmdBr,
    &processCmdMovb,
    &processCmdBeq,
    &processCmdBne,
    &processCmdInc,
    &processCmdDec,
    &processCmdHalt,
    &processCmdRti,
    &processCmdJmp,
    &processCmdMul,
    &processCmdAdd,
    &processCmdNop
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
        case CMD_BNE : return "bne";
        case CMD_INC : return "inc";
        case CMD_DEC : return "dec";
        case CMD_HALT: return "halt";
        case CMD_RTI : return "rti";
        case CMD_JMP : return "jmp";
        case CMD_MUL : return "mul";
        case CMD_ADD : return "add";
        case CMD_NOP : return "nop";
        default:       return "";
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
    if(strCompare(str, "mov")       ) return CMD_MOV;
    else if(strCompare(str, "clr")  ) return CMD_CLR;
    else if(strCompare(str, "movb") ) return CMD_MOVB;
    else if(strCompare(str, "inc")  ) return CMD_INC;
    else if(strCompare(str, "dec")  ) return CMD_DEC;
    else if(strCompare(str, "br")   ) return CMD_BR;
    else if(strCompare(str, "bne")  ) return CMD_BNE;
    else if(strCompare(str, "done:")) return CMD_HALT;
    else if(strCompare(str, "beq")  ) return CMD_BEQ;
    else if(strCompare(str, "rti")  ) return CMD_RTI;
    else if(strCompare(str, "jmp")  ) return CMD_JMP;
    else if(strCompare(str, "mul")  ) return CMD_MUL;
    else if(strCompare(str, "add")  ) return CMD_ADD;
    else if(strCompare(str, "nop")  ) return CMD_NOP;
    else                              return CMD_UNKNOWN;
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

int getType(const char* str)
{
    int len = strlen(str);
    if(isMacro(str))
        return 1;
    else if(isRegister(str))
        return 2;
    else if((str[0] == '@') && (str[1] == '#'))
        return 3;
    else if((str[0] == '-'))
        return 4;
    else if((str[0] == '(') && (str[len-1] == '+' ))
        return 5;
    else if((str[0] == '(') && (str[len-1] == ')' ))
        return 6;
    else
        return 0;
}

void processCmdMov(CmdStructPtr cmd)
{
    uint32_t optcode = opcodes[OP_MOV].base;
    uint32_t word   = 0x00;

    int flag1 = getType(cmd->param1);
    int flag2 = getType(cmd->param2);

    if((flag1 == 1) && (flag2 == 2))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        word   = parseAttributeInCommand(cmd->param2);
        arrayPush(BUILD_DO(optcode, 2, 0x7, 0, word));

        arrayPush(cmd->address);
        cmd->address += 2;

        word = parseAttributeInCommand(cmd->param1);
        arrayPush(word);
    }
    else if((flag1 == 1) && (flag2 == 3))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 2, 0x7, 3, 0x7));

        arrayPush(cmd->address);
        cmd->address += 2;

        word = parseAttributeInCommand(cmd->param1);
        arrayPush(word);

        arrayPush(cmd->address);
        cmd->address += 2;

        word = parseAttributeInCommand(cmd->param2+1);
        arrayPush(word);
    }
    else if((flag1 == 2) && (flag2 == 4))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 0, getRegAddr(cmd->param1), 4, 0x6));
    }
    else if((flag1 == 5) && (flag2 == 2))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 0x2, 0x6, 0x00, getRegAddr(cmd->param2)));
    }
    else if((flag1 == 3) || (flag2 == 2))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 3, 0x7, 0, getRegAddr(cmd->param2)));

        arrayPush(cmd->address);
        cmd->address += 2;

        word = parseAttributeInCommand(cmd->param1+1);
        arrayPush(word);
    }
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

void processCmdBne(CmdStructPtr cmd)
{
    uint16_t val = opcodes[OP_BNE].base;
    val |= calcOffsetForBr(cmd->address, cmd->param1);

    arrayPush(cmd->address);
    arrayPush(val);

    cmd->address += 2;
}

void processCmdMovb(CmdStructPtr cmd)
{
    int flag1 = getType(cmd->param1);
    int flag2 = getType(cmd->param2);

    int optcode = opcodes[OP_MOVB].base;
    int mode    = 2;
    int reg1    = getRegAddr("r1");
    int reg2    = getRegAddr("r2");

    if((flag1 == 5) && (flag2 == 5))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, mode, reg1, mode, reg2));
    }
    else if((flag1 == 3) && (flag2 == 2))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 3, 7, 0, getRegAddr(cmd->param2)));

        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(0xFF72);
    }
    else if((flag1 == 5) && (flag2 == 2))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 2, 0, 1, getRegAddr(cmd->param2)));
    }
    else if((flag1 == 5) && (flag2 == 6))
    {
        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(BUILD_DO(optcode, 2, 0, 1, getRegAddr("r1")));
    }
}

void processCmdInc(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    arrayPush(opcodes[OP_INC].base | getRegAddr(cmd->param1));

    cmd->address += 2;
}

void processCmdDec(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    arrayPush(opcodes[OP_DEC].base | getRegAddr(cmd->param1));

    cmd->address += 2;
}

void processCmdHalt(CmdStructPtr cmd)
{
    afterBeq(cmd->address);

    arrayPush(cmd->address);
    arrayPush(opcodes[OP_HALT].base);

    cmd->address += 2;
}

void processCmdRti(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    arrayPush(opcodes[OP_RTI].base);

    cmd->address += 2;
}

void processCmdJmp(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    cmd->address += 2;

    arrayPush(opcodes[OP_JMP].base | 0x57);

    arrayPush(cmd->address);
    cmd->address += 2;

    arrayPush(0x244);
}

void processCmdMul(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    cmd->address += 2;

    arrayPush(opcodes[OP_MUL].base | 0x17);

    arrayPush(cmd->address);
    cmd->address += 2;

    arrayPush(0x08);
}

void processCmdAdd(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    cmd->address += 2;

    if(strcmp(cmd->param1, "#fonts") == 0)
    {
        arrayPush(0x6DC0);

        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(0x1FC0);
    }
    else if(strcmp(cmd->param1, "#40") == 0)
    {
        arrayPush(0x65C0);

        arrayPush(cmd->address);
        cmd->address += 2;

        arrayPush(0x20);
    }
}

void processCmdNop(CmdStructPtr cmd)
{
    arrayPush(cmd->address);
    arrayPush(0xA0);

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
