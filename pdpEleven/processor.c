#include "processor.h"
#include <stdio.h>

#include "test_program.h"

#define DEBUG_MODE

// Flags

#define _C        (1 << 0)
#define _V        (1 << 1)
#define _Z        (1 << 2)
#define _N        (1 << 3)
#define _T        (1 << 4)

#define GET_(X)   (flags & X)
#define SET_(X)   flags |= X
#define CLEAR_(X) flags &= ~X

#define SET_IF(F, COND) if(COND) SET_(F); else CLEAR_(F);

// Bits

#define BIT3     07
#define BIT6     077
#define SHIFT15  017
#define SHIFT7   07
#define BITMAX   0100000

#define ODIGIT(X,N)  (BIT3 & (X >> (3*N)))
#define REG(X)       (X & BIT6)
#define IS_BYTE(X)   (X >> SHIFT15)

#define PC_REG   07
#define REG_NUMBER 8



// pointers to memory

uint16_t registers[REG_NUMBER];
uint8_t *memory_ = (uint8_t *) programm_;
uint8_t flags;

uint8_t *getMemory(uint16_t address) { return memory_ + address; }
uint16_t *getRegister(uint8_t ind) { return registers + ind; }

uint16_t nextWord(int inc) {
    uint16_t *pc = getRegister(PC_REG);
    if(inc) *pc += 02;

    uint16_t addr = *pc, mem;
    mem = *((uint16_t*) getMemory(addr));

#ifdef DEBUG_MODE
    printf("%o\t%6o\t", addr, mem);
#endif

    return mem;
}

void resetRegisters() {
    int i;
    for(i = 0; i < REG_NUMBER; ++i) {
        registers[i] = 0;
    }
}

void resetFlags() {
    flags = 0;
}

// Memory modes
// Register mode
uint16_t *mode_0(uint8_t ind) { return getRegister(ind); }
uint8_t *mode_0b(uint8_t ind) { return (uint8_t*) getRegister(ind); }



// Autoincrement mode
uint16_t* mode_2(uint8_t ind) {
    uint16_t addr = *getRegister(ind);
    *getRegister(ind) += 2;
    return (uint16_t*) getMemory(addr);
}
uint8_t* mode_2b(uint8_t ind) {
    uint16_t addr = *getRegister(ind);
    *getRegister(ind) += 1;
    return getMemory(addr);
}
// pc2 mode
uint16_t* mode_2pc() {
    *getRegister(PC_REG) += 2;
    return (uint16_t *) getMemory(*getRegister(PC_REG));
}

// Autodecrement mode
uint16_t* mode_4(uint8_t ind) {
    *getRegister(ind) -= 2;
    uint16_t addr = *getRegister(ind);
    return (uint16_t *) getMemory(addr);
}
uint8_t* mode_4b(uint8_t ind) {
    *getRegister(ind) -= 1;
    uint16_t addr = *getRegister(ind);
    return getMemory(addr);
}

// Index mode
uint8_t* mode_6b(uint8_t ind) {
    uint16_t addr = nextWord(0);
    addr += *getRegister(ind);
    return getMemory(addr);
}
uint16_t *mode_6(uint8_t ind) {
    return (uint16_t*) mode_6b(ind);
}



// Inderect Modes
// Register deferred
uint8_t* mode_1b(uint8_t ind) {
    uint16_t addr = *getRegister(ind);
    return getMemory(addr);
}
uint16_t *mode_1(uint8_t ind) {
    return (uint16_t *) mode_1b(ind);
}

// Autoincrement deferred
uint8_t* mode_3b(uint8_t ind) {
    uint16_t addr = *getRegister(ind);
    *getRegister(ind) += 2;
    addr = *((uint16_t*) getMemory(addr));
    return getMemory(addr);
}
uint16_t* mode_3(uint8_t ind) {
    return (uint16_t*) mode_3b(ind);
}
uint16_t* mode_3pc() {
    *getRegister(PC_REG) += 2;
    uint16_t addr = *((uint16_t *) getMemory(*getRegister(PC_REG)));
    return (uint16_t *) getMemory(addr);
}

// Autodecrement deferred
uint8_t *mode_5b(uint8_t ind) {
    *getRegister(ind) -= 2;
    uint16_t addr = *getRegister(ind);
    addr = *((uint16_t*) getMemory(addr));
    return getMemory(addr);
}
uint16_t *mode_5(uint8_t ind) {
    return (uint16_t*) mode_5b(ind);
}
uint16_t *mode_5pc() {
    *getRegister(PC_REG) += 2;
    uint16_t addr = *((uint16_t*) getMemory(*getRegister(PC_REG)));
    *getRegister(PC_REG) += 2;
    addr += *getRegister(PC_REG);
    return (uint16_t *) getMemory(addr);
}

// Index deferred
uint8_t *mode_7b(uint8_t ind) {
    uint16_t addr = nextWord(0);
    addr += *getRegister(ind);
    addr = *((uint16_t*) getMemory(addr));
    return getMemory(addr);
}
uint16_t *mode_7(uint8_t ind) {
    return (uint16_t*) mode_7b(ind);
}
uint16_t *mode_7pc() {
    uint16_t addr = *mode_5pc();
    return (uint16_t *) getMemory(addr);
}

uint16_t* getWord(uint16_t op) {
    //printf("%o %o ", ODIGIT(op,1), ODIGIT(op,0));
    switch(ODIGIT(op, 1)) {
    case 00: return mode_0(ODIGIT(op,0));
    case 01: return mode_1(ODIGIT(op,0));
    case 02:
        return (ODIGIT(op,0) == 07) ? mode_2pc() : mode_2(ODIGIT(op,0));
    case 03:
        return (ODIGIT(op,0) == 07) ? mode_3pc() : mode_3(ODIGIT(op,0));
    case 04: return mode_4(ODIGIT(op,0));
    case 05:
        return (ODIGIT(op,0) == 07) ? mode_5pc() : mode_5(ODIGIT(op,0));
    case 06: return mode_6(ODIGIT(op,0));
    case 07:
        return (ODIGIT(op,0) == 07) ? mode_7pc() : mode_7(ODIGIT(op,0));
    default:
        break;
    }
    return NULL;
}

uint8_t* getByte(uint16_t op) {
    //printf("%o %o ", ODIGIT(op,1), ODIGIT(op,0));
    switch(ODIGIT(op, 1)) {
    case 00: return mode_0b(ODIGIT(op,0));
    case 01: return mode_1b(ODIGIT(op,0));
    case 02: return mode_2b(ODIGIT(op,0));
    case 03: return mode_3b(ODIGIT(op,0));
    case 04: return mode_4b(ODIGIT(op,0));
    case 05: return mode_5b(ODIGIT(op,0));
    case 06: return mode_6b(ODIGIT(op,0));
    case 07: return mode_7b(ODIGIT(op,0));
    default:
        break;
    }
    return NULL;
}

uint16_t* toWordPtr(uint16_t op) {
    uint16_t* ptr16;
    if(IS_BYTE(op)) {
        uint8_t* ptr8 = getByte(op);
        if(((int)ptr8) & 1)
            ptr16 = (uint16_t*) (ptr8-1);
        else
            ptr16 = (uint16_t*) ptr8;
    }
    else
        ptr16 = getWord(op);

    return ptr16;
}

// Clear dst
void _clr(uint16_t op) {
    CLEAR_(_N); SET_(_Z); CLEAR_(_V); CLEAR_(_C);

    if(IS_BYTE(op))
        *getByte(op) = 0;
    else
        *getWord(op) = 0;

    //printf(" -%o- ", *getWord(op));

#ifdef DEBUG_MODE
    puts("CLR");
#endif // DEBUG_MODE
}

// Complement dst
void _com(uint16_t op) {
    CLEAR_(_V); SET_(_C);

    uint16_t *wPtr = toWordPtr(op);

    if(IS_BYTE(op)) {
        uint8_t tmp = *getByte(op);
        tmp = ~tmp;
        *getByte(op) = tmp;
    }
    else {
        uint16_t tmp = *getWord(op);
        tmp = ~tmp;
        *getWord(op) = tmp;
    }

    SET_IF(_Z, (*wPtr)==0);
    SET_IF(_N, IS_BYTE(*wPtr));

#ifdef DEBUG_MODE
    puts("COM");
#endif // DEBUG_MODE
}

// Increment dst
void _inc(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);
    SET_IF(_V, *wPtr==077777);

    if(IS_BYTE(op))
        *getByte(op) += 1;
    else
        *getWord(op) += 1;

    SET_IF(_Z, (*wPtr)==0);
    SET_IF(_N, IS_BYTE(*wPtr));

#ifdef DEBUG_MODE
    puts("INC");
#endif // DEBUG_MODE
}

// Decrement dst
void _dec(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);
    SET_IF(_V, *wPtr==BITMAX);

    if(IS_BYTE(op))
        *getByte(op) -= 1;
    else
        *getWord(op) -= 1;

    SET_IF(_Z, *wPtr==0);
    SET_IF(_N, IS_BYTE(*wPtr));

#ifdef DEBUG_MODE
    puts("DEC");
#endif // DEBUG_MODE
}

// Negate dst
void _neg(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);

    if(IS_BYTE(op)) {
        uint8_t* tmp = getByte(op);
        *tmp = -(*tmp);
    }
    else {
        uint16_t* tmp = getWord(op);
        *tmp = -(*tmp);
    }

    SET_IF(_N, IS_BYTE(*wPtr));
    SET_IF(_Z, *wPtr==0);
    SET_IF(_V, *wPtr==BITMAX);  // need to correct
    SET_IF(_C, *wPtr!=0);

#ifdef DEBUG_MODE
    puts("NEG");
#endif // DEBUG_MODE
}

// Test dst
void _tst(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);

    CLEAR_(_V); CLEAR_(_V);
    SET_IF(_N, IS_BYTE(*wPtr));
    SET_IF(_Z, *wPtr==0);

#ifdef DEBUG_MODE
    puts("TST");
#endif // DEBUG_MODE
}

void _mov(uint16_t op) {
    CLEAR_(_V);
    if(IS_BYTE(op)) {
        uint8_t *dst = getByte(op);
        uint8_t *src = getByte(op >> 06);
        *dst = *src;
        SET_IF(_Z, *src == 0);
        SET_IF(_N, (*src) >> SHIFT7);
    }
    else {
        uint16_t *dst = getWord(op);
        uint16_t *src = getWord(op >> 06);
        *dst = *src;
        //printf(" -%6o- ", *src);
        SET_IF(_Z, *src == 0);
        SET_IF(_N, IS_BYTE(*src));
    }
#ifdef DEBUG_MODE
    puts(IS_BYTE(op) ? "MOVB" : "MOV");
#endif // DEBUG_MODE
}

void _beq(uint16_t op) {
    if(GET_(_N)) {
        uint16_t *pc = getRegister(07);
        uint16_t offset = op & ((1 << 010)-1);
        *pc += offset << 1;  // 2*offset
    }

#ifdef DEBUG_MODE
    puts("BEQ");
#endif // DEBUG_MODE
}

void _br(uint16_t op) {
    uint16_t *pc = getRegister(07);
    uint16_t offset = op & ((1 << 010)-1);
    *pc += (offset << 1) + 02;

#ifdef DEBUG_MODE
    puts("BR");
#endif // DEBUG_MODE
}

int eval(uint16_t opcode) {
    /*
    uint16_t wopcode = opcode & ~(1 << SHIFT7);
    if     ((wopcode >> 06) & 050) { _clr(opcode); }  // CLR
    else if((wopcode >> 06) & 052) { _inc(opcode); }  // INC
    else if((wopcode >> 014) & 01) { _mov(opcode); }  // MOV
    else if((wopcode >> 010) & 03) { _beq(opcode); }  // BEQ
    else if((wopcode >> 010) & 01) { _br(opcode);  }  // BR

    else { return 0; }
    */
    if((opcode >= 0005000 && opcode < 0005100) || (opcode >= 0105000 && opcode < 0105100))
    { _clr(opcode); } // CLR
    else if((opcode >= 0005200 && opcode < 0005300) || (opcode >= 0105200 && opcode < 0105300))
    { _inc(opcode); } // INC
    else if((opcode >= 0010000 && opcode < 0020000) || (opcode >= 0110000 && opcode < 0120000))
    { _mov(opcode); } // MOV
    else if(opcode >= 0001400 && opcode < 0001500)
    { _beq(opcode); } // BEQ
    else if(opcode >= 0000400 && opcode < 0001000)
    { _br(opcode); }  // BR

    else { return 0; }


    return 1;
}

void printRegisters() {
    printf("\nR0:%o R1:%o R2:%o R3:%o R4:%o R5:%o R6:%o R7:%o\n",
           registers[0], registers[1], registers[2], registers[3], registers[4],
            registers[5], registers[6], registers[7]);
}

void testProcessor() {

    uint16_t opcode;
    int step;

    resetFlags();
    resetRegisters();

    opcode = nextWord(0);
    printRegisters();

    for(step = 0; step < 10; ++step) {
    //while((opcode = nextWord()) != HALT) {

        eval(opcode);
        printRegisters();
        opcode = nextWord(1);
        //printf("\n");
    }

}

