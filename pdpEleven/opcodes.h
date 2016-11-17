#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

#define MSK_1    07
#define MSK_3    0700
#define MSK_12   077
#define MSK_34   07700
#define MSK_BYTE 0377

typedef struct Opcodes {
    uint16_t base;
    uint16_t src_mask;
    uint16_t dst_mask;
    uint16_t offset_mask;
    uint16_t isbyte;
    uint16_t tacts;
    const char *name;
} Opcodes;

extern Opcodes opcodes[];
typedef enum _OPCODELIST {
    OP_HALT,
    OP_WAIT,
    OP_RTI,
    OP_BPI,
    OP_IOT,
    OP_RESET,
    OP_RTT,
    OP_JMP,
    OP_RTS,
    OP_CLC,
    OP_SEC,
    OP_SWAB,
    OP_BR,
    OP_BNE,
    OP_BEQ,
    OP_BGE,
    OP_BLT,
    OP_BGT,
    OP_BLE,
    OP_JSR,
    OP_CLR,
    OP_COM,
    OP_INC,
    OP_DEC,
    OP_NEG,
    OP_ADC,
    OP_SBC,
    OP_TST,
    OP_ROR,
    OP_ROL,
    OP_ASR,
    OP_ASL,
    OP_MARK,
    OP_MFPI,
    OP_MTPI,
    OP_SXT,
    OP_MOV,
    OP_CMP,
    OP_BIT,
    OP_BIC,
    OP_BIS,
    OP_ADD,
    OP_MUL,
    OP_DIV,
    OP_ASH,
    OP_ASHC,
    OP_XOR,
    OP_SOB,
    OP_BPL,
    OP_BMI,
    OP_BHI,
    OP_BLOS,
    OP_BVC,
    OP_BVS,
    OP_BCC,
    OP_BCS,
    OP_EMT,
    OP_TRAP,
    OP_CLRB,
    OP_COMB,
    OP_INCB,
    OP_DECB,
    OP_NEGB,
    OP_ADCB,
    OP_SBCB,
    OP_TSTB,
    OP_RORB,
    OP_ROLB,
    OP_ASRB,
    OP_ASLB,
    OP_MOVB,
    OP_CMPB,
    OP_BITB,
    OP_BICB,
    OP_BISB,
    OP_SUB,
    OP_COUNT
} OPCODELIST;


#endif // OPCODES_H
