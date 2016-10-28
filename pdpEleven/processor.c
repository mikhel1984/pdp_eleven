#include "processor.h"
#include <stdio.h>

#include "test_program.h"

//#define DEBUG_MODE

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

// test
void print8(uint8_t val) {
    int state, i;
    for(i = 7; i>= 0; --i) {
        state = val & (1 << i);
        printf("%d%s", state ? 1 : 0, i%4 ? "" : " ");
    }
}

void print16(uint16_t val) {
    int state, i;
    for(i = 15; i >= 0; --i) {
        state = val & (1 << i);
        printf("%d%s", state ? 1 : 0, i%4 ? "" : " ");
    }
}


// additional code

uint8_t inv_code8(uint8_t val) {
    return (~val|017)+1;
}

uint16_t inv_code16(uint16_t val) {
    return ((~val|0377)+1);
}

uint16_t add_code16(uint16_t val) {
    return (IS_BYTE(val)) ? inv_code16(val) : val;
}

uint16_t convert16t(uint8_t val) {
    if(val >> 07) {
        return 0177400 + (uint16_t) val;
    }
    return (uint16_t) val;
}

uint16_t sum16(uint16_t v1, uint16_t v2) {
    return v1 + v2;
}

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
int _clr(uint16_t op) {
    CLEAR_(_N); SET_(_Z); CLEAR_(_V); CLEAR_(_C);

    if(IS_BYTE(op))
        *getByte(op) = 0;
    else
        *getWord(op) = 0;

    //printf(" -%o- ", *getWord(op));

#ifdef DEBUG_MODE
    puts("CLR");
#endif // DEBUG_MODE
    return 1;
}

// Complement dst
int _com(uint16_t op) {
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
    return 1;
}

// Increment dst
int _inc(uint16_t op) {
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
    return 1;
}

// Decrement dst
int _dec(uint16_t op) {
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
    return 1;
}

// Negate dst
int _neg(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);

    if(IS_BYTE(op)) {
        uint8_t* tmp = getByte(op);
        //*tmp = -(*tmp);
        *tmp = inv_code8(*tmp);
    }
    else {
        uint16_t* tmp = getWord(op);
        //*tmp = -(*tmp);
        *tmp = inv_code16(*tmp);
    }

    SET_IF(_N, IS_BYTE(*wPtr));
    SET_IF(_Z, *wPtr==0);
    SET_IF(_V, *wPtr==BITMAX);  // need to correct
    SET_IF(_C, *wPtr!=0);

#ifdef DEBUG_MODE
    puts("NEG");
#endif // DEBUG_MODE
    return 1;
}

// Test dst
int _tst(uint16_t op) {
    uint16_t *wPtr = toWordPtr(op);

    CLEAR_(_V); CLEAR_(_V);
    SET_IF(_N, IS_BYTE(*wPtr));
    SET_IF(_Z, *wPtr==0);

#ifdef DEBUG_MODE
    puts("TST");
#endif // DEBUG_MODE
    return 1;
}

int _mov(uint16_t op) {
    CLEAR_(_V);
    if(IS_BYTE(op)) {
        uint8_t *dst = getByte(op);
        uint8_t *src = getByte(op >> 06);
        *dst = *src;
        printf(" %c ", (char) *src);
        SET_IF(_Z, *src == 0);
        SET_IF(_N, (*src) >> SHIFT7);
    }
    else {
        uint16_t *dst = getWord(op);
        uint16_t *src = getWord(op >> 06);
        *dst = *src;        
        SET_IF(_Z, *src == 0);
        SET_IF(_N, IS_BYTE(*src));
    }
#ifdef DEBUG_MODE
    puts(IS_BYTE(op) ? "MOVB" : "MOV");
#endif // DEBUG_MODE
    return 1;
}

int _beq(uint16_t op) {
#ifdef DEBUG_MODE
    puts("BEQ");
#endif // DEBUG_MODE
    if(GET_(_Z)) {
        uint16_t *pc = getRegister(PC_REG);
        uint8_t offset8 = (uint8_t) op;
        uint16_t offset = convert16t(offset8);
        *pc += 02;
        *pc += offset;
        *pc += offset;
        return 0;
    }
    return 1;
}

int _br(uint16_t op) {
    uint16_t *pc = getRegister(PC_REG);
    uint8_t offset8 = (uint8_t) op;
    uint16_t offset = convert16t(offset8);
    *pc += 02;
    *pc += offset;
    *pc += offset;

#ifdef DEBUG_MODE
    puts("BR");
#endif // DEBUG_MODE
    return 0;
}



int eval(uint16_t opcode) {

    if((opcode >= 0005000 && opcode < 0005100) || (opcode >= 0105000 && opcode < 0105100))
    { return _clr(opcode); } // CLR
    else if((opcode >= 0005200 && opcode < 0005300) || (opcode >= 0105200 && opcode < 0105300))
    { return _inc(opcode); } // INC
    else if((opcode >= 0010000 && opcode < 0020000) || (opcode >= 0110000 && opcode < 0120000))
    { return _mov(opcode); } // MOV
    else if(opcode >= 0001400 && opcode < 0001500)
    { return _beq(opcode); } // BEQ
    else if(opcode >= 0000400 && opcode < 0001000)
    { return _br(opcode); }  // BR

    return 1;
}

void printRegisters() {
    printf("\nR0:%o R1:%o R2:%o R3:%o R4:%o R5:%o R6:%o R7:%o\n",
           registers[0], registers[1], registers[2], registers[3], registers[4],
            registers[5], registers[6], registers[7]);
}

void testProcessor() {

    uint16_t opcode;
    int step = 0, inc_address = 0, i;

    resetFlags();
    resetRegisters();

    // test string(s)
    uint8_t *mem = (uint8_t*) programm_;
    for(i = 20; i < 108; ++i) {
        printf("%c", (char) mem[i]);
    }
    printf("\n");

    opcode = nextWord(inc_address);
    printRegisters();


    //for(step = 0; step < 170; ++step) {
    while(opcode != HALT) {
    //while((opcode = nextWord()) != HALT) {

        inc_address = eval(opcode);
        printRegisters();
        opcode = nextWord(inc_address);
        //printf("\n");
        step ++;
    }
    // after work
    printf("\nNumber of circles: %d\n", step);
    printf("\n");
    for(i = 20; i < 108; ++i) {
        printf("%c", (char) mem[i]);
    }
    printf("\n");

}

