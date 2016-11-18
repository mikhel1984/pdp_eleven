#include "processor.h"
#include <stdio.h>
#include <string.h>
#include "opcodes.h"
#include "memory.h"
#include <time.h>

#include "test_program.h"

//#define WRITELOG

// Flags

#define GET_(X)   (flags & X)
#define SET_(X)   flags |= X
#define CLEAR_(X) flags &= ~X

#define SET_IF(F, COND) if(COND) SET_(F); else CLEAR_(F);

#define PIPE_NUMBER_MAX 8

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

typedef struct _Instruction Instruction;
typedef int (*Eval)(Instruction*);

// Vector of processor operations
Eval functionList[OP_COUNT] = {NULL};

typedef enum _PIPESTEP {PIPE_FETCH, PIPE_DECODE, PIPE_FETCH_OP,
                        PIPE_EVAL, PIPE_WRITE, PIPE_END, PIPE_HALT} PIPESTEP;

// Data for exchanging and control in pipeline
struct _Instruction {
    uint16_t code;        // instruction
    OPCODELIST index;     // index of operation
    uint8_t src_val[2];   // copy of source data
    uint8_t dst_val[2];   // copy of destination data
    uint8_t *src_ptr;     // source address
    uint8_t *dst_ptr;     // destination address
    // multiscalar
    PIPESTEP step;        // current step of processing
    PIPESTEP delay;       // delay for next pipes
    uint8_t tacts;        // current step duration
    uint8_t execute;      // apply method of current step (for tact elimination)    
};

Instruction initInstruction(void) {
    Instruction inst = {0, 0, {0,0}, {0,0}, NULL, NULL, PIPE_FETCH, PIPE_FETCH, 1, 1};
    return inst;
}

Instruction pipes[PIPE_NUMBER_MAX];
Instruction storePipes[PIPE_NUMBER_MAX];

char lastInstruction[256] = "";

char* getLastInstruction() { return lastInstruction; }

// print bits of number (for testing)
void print8(uint8_t val) {
    int state, i;
    for(i = 7; i>= 0; --i) {
        state = val & (1 << i);
        printf("%d%s", state ? 1 : 0, i%4 ? "" : " ");
    }
}

void nextStep(Instruction *inst) {
    if(inst->delay == PIPE_HALT)
        inst->step = PIPE_HALT;
    else
        inst->step ++;

    inst->execute = true;
    if(inst->delay <= inst->step)
        inst->delay = inst->step;
}


#ifdef WRITELOG
#define LOGFILE "proc.log"
char logging[256] = "";

void writelog(const char *fname, const char *txt) {
    FILE *file = fopen(fname, "a");
    if(file) {
        fprintf(file, "%s\n", txt);
        fclose(file);
    }

}
#endif // LOG

void print16(uint16_t val) {
    int state, i;
    for(i = 15; i >= 0; --i) {
        state = val & (1 << i);
        printf("%d%s", state ? 1 : 0, i%4 ? "" : " ");
    }
}


// working with additional code
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

//                      MEMORY
// pointers to memory

uint16_t registers[REG_NUMBER];
uint16_t storeRegisters[REG_NUMBER];

uint8_t *memory_ = (uint8_t *) programm_;
uint8_t flags;
uint8_t storeFlags;

//uint8_t *getMemory(uint16_t address) { return memory_ + address; }
uint8_t *getMemory(uint16_t address) { return memoryGetPointer() + address; }

uint16_t *getRegister(uint8_t ind) { return registers + ind; }

uint8_t getFlags() { return flags; }

void setProgrammStart(uint16_t ind) {
    *getRegister(PC_REG) = ind;
}

// Get data from memory
uint16_t fetchMem(Instruction *inst) {
    uint16_t addr = *getRegister(PC_REG);
    uint16_t opcode = *((uint16_t*) getMemory(addr));

    if(inst) {
        inst->code = opcode;
        inst->execute = 0;
        inst->tacts = 1;
    }

    return opcode;
}

void incrementPC() {
    *getRegister(PC_REG) += 02;
}

uint16_t nextWord(int inc) {
    if(inc) incrementPC();
    return fetchMem(NULL);
}

// prepare
void resetRegisters() {
    int i;
    for(i = 0; i < REG_NUMBER; ++i) {
        registers[i] = 0;
    }
}

void resetFlags() {
    flags = 0;
}

//                      Memory modes
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
    incrementPC();
    uint16_t addr = fetchMem(NULL);
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
    incrementPC();
    uint16_t addr = fetchMem(NULL);
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

// get operand using mask
uint16_t getOperand(uint16_t code, uint16_t mask) {
    if(mask == 0) return 0;

    code &= mask;
    while(!(mask & 01)) {
        mask >>= 1;
        code >>= 1;
    }

    return code;
}

// Etaps of pipeline
int fetchOperands(Instruction *inst) {
    OPCODELIST op = inst->index;
    uint16_t mask;
    inst->tacts = 1;
    // read if get operand
    if((mask = opcodes[op].src_mask) != 0) {
        if(opcodes[op].isbyte) {
            inst->src_ptr = getByte(getOperand(inst->code, mask));
            *(inst->src_val) = *(inst->src_ptr);
        }
        else {  // word
            inst->src_ptr = (uint8_t*) getWord(getOperand(inst->code, mask));
            *((uint16_t*) inst->src_val) = *((uint16_t*) inst->src_ptr);
        }
        if(ODIGIT(getOperand(inst->code, mask), 2) != 0) inst->tacts++;
    }
    // same for destination
    if((mask = opcodes[op].dst_mask) != 0) {
        if(opcodes[op].isbyte) {
            inst->dst_ptr = getByte(getOperand(inst->code, mask));
            *(inst->dst_val) = *(inst->dst_ptr);
        }
        else {            
            inst->dst_ptr = (uint8_t*) getWord(getOperand(inst->code, mask));
            *((uint16_t*) inst->dst_val) = *((uint16_t*) inst->dst_ptr);
        }
        if(ODIGIT(getOperand(inst->code, mask), 2) != 0) inst->tacts++;
    }

    if(inst->execute && inst->delay == PIPE_EVAL)
        incrementPC();

    inst->execute = 0;
/*
#ifdef WRITELOG
    sprintf(logging, "FETCHOP: src=%o dst=%o", (int) inst->src_val, (int)inst->dst_val);
    writelog(LOGFILE, logging);
#endif
*/
    return 1;

}

int isJump(Instruction* inst) {
    return (inst->index >= OP_BR && inst->index <= OP_BLE) || inst->index == OP_JMP;
}

// Write data after evaluation
int writeOperands(Instruction *inst) {
    OPCODELIST op = inst->index;
    inst->tacts = 1;
    // write only if get destination
    if(inst->dst_ptr) {
        if(opcodes[op].isbyte)
            *(inst->dst_ptr) = *(inst->dst_val);
        else
            *((uint16_t*) inst->dst_ptr) = *((uint16_t*) inst->dst_val);
        uint16_t mask = opcodes[op].dst_mask;
        if(ODIGIT(getOperand(inst->code, mask), 2) != 0) inst->tacts++;
    }

    inst->execute = 0;

#ifdef WRITELOG    
    writelog(LOGFILE, "");
#endif

    return 1;
}

// Delay processor frequency
void timeNop(uint8_t t_ms) {
    clock_t t0 = clock();
    long i = 1;
    int delay = t_ms * CLOCKS_PER_SEC / 1000;

    while(clock() < t0 + delay) {
        i += 1;
    }

}


int clr8(Instruction *inst) {
    CLEAR_(_N); SET_(_Z); CLEAR_(_V); CLEAR_(_C);    
    *(inst->dst_val) = 0;
    return 1;
}

int clr16(Instruction *inst) {
    CLEAR_(_N); SET_(_Z); CLEAR_(_V); CLEAR_(_C);

    *((uint16_t*) inst->dst_val) = 0;
    return 1;
}

int inc8(Instruction *inst) {

    uint8_t *val = inst->dst_val;
    SET_IF(_V, *val == 0177);
    *val += 1;
    SET_IF(_N, (*val) >> SHIFT7);
    SET_IF(_Z, *val == 0);
    return 1;
}

int inc16(Instruction *inst) {
    uint16_t *val = (uint16_t*) inst->dst_val;

    SET_IF(_V, *val == 077777);
    *val += 1;
    SET_IF(_N, IS_BYTE(*val));
    SET_IF(_Z, *val == 0);
    return 1;
}

int dec8(Instruction *inst) {
    uint8_t *val = inst->dst_val;
    SET_IF(_V, *val == 0200);
    *val -= 1;
    SET_IF(_N, (*val) >> SHIFT7);
    SET_IF(_Z, *val == 0);
    return 1;
}

int dec16(Instruction *inst) {
    uint16_t *val = (uint16_t*) inst->dst_val;
    SET_IF(_V, *val == 0100000);
    *val -= 1;
    SET_IF(_N, IS_BYTE(*val));
    SET_IF(_Z, *val == 0);
    return 1;
}

int mov8(Instruction* inst) {
    CLEAR_(_V);
    *(inst->dst_val) = *(inst->src_val);
    SET_IF(_Z, *(inst->src_val) == 0);
    SET_IF(_N, *(inst->src_val) >> SHIFT7);
    return 1;
}

int mov16(Instruction *inst) {
    uint16_t *src = (uint16_t*) inst->src_val, *dst = (uint16_t*) inst->dst_val;
    CLEAR_(_V);
    *dst = *src;
    SET_IF(_Z, *src == 0);
    SET_IF(_N, IS_BYTE(*src));
    return 1;
}

int beq8(Instruction *inst) {
    if(GET_(_Z)) {        
        uint8_t val = (uint8_t) getOperand(inst->code, opcodes[inst->index].offset_mask);
        uint16_t *pc = getRegister(PC_REG);
        uint16_t offset = convert16t(val);
        *pc += 02 + offset + offset;
        return 0;
    }
    return 1;
}

int bne8(Instruction* inst) {
    if(!GET_(_Z)) {
        uint8_t val = (uint8_t) getOperand(inst->code, opcodes[inst->index].offset_mask);
        uint16_t *pc = getRegister(PC_REG);
        uint16_t offset = convert16t(val);
        *pc += 02 + offset + offset;
        return 0;
    }
    return 1;
}

int br8(Instruction *inst) {
    uint8_t val = (uint8_t) getOperand(inst->code, opcodes[inst->index].offset_mask);
    uint16_t *pc = getRegister(PC_REG);
    uint16_t offset = convert16t(val);
    *pc += 02 + offset + offset;
    return 0;
}
// if function is not implemented
int gag(Instruction *inst) {
    return inst ? 1 : 0;
}

void initializeFunctions() {
    functionList[OP_CLR] = clr16;
    functionList[OP_CLRB] = clr8;
    functionList[OP_INC] = inc16;
    functionList[OP_INCB] = inc8;
    functionList[OP_DEC] = dec16;
    functionList[OP_DECB] = dec8;
    functionList[OP_MOV] = mov16;
    functionList[OP_MOVB] = mov8;
    functionList[OP_BEQ] = beq8;
    functionList[OP_BNE] = bne8;
    functionList[OP_BR] = br8;
    int i = 0;
    for(i = 0; i < OP_COUNT; ++i) {
        if(!functionList[i]) functionList[i] = gag;
    }
}

// define opcode and operands
int decode(Instruction *res) {
    int ind = 0;
    //Instruction res;
    uint16_t opcode = res->code;

    while(opcode >= opcodes[ind].base) { ind++; }
    ind--;

    res->index = (OPCODELIST) ind;

    if(res->index == OP_HALT) {
        res->delay = PIPE_HALT;
    }
    else if(isJump(res)) {
        res->delay = PIPE_WRITE;
    }    
    else if(ODIGIT(opcode, 2) == 06 || ODIGIT(opcode, 2) == 07 ||
            ODIGIT(opcode, 4) == 06 || ODIGIT(opcode, 4) == 07) {
        res->delay = PIPE_EVAL;
    }    
    else {
        res->delay = PIPE_DECODE;
        if(res->execute)
            incrementPC();
    }
    res->tacts = 1;
    res->execute = 0;

    sprintf(lastInstruction, "%d %o %s\n", *getRegister(PC_REG), opcode, opcodes[res->index].name);

/*
#ifdef WRITELOG
    sprintf(logging, "DECODE: index=%o opcode=%o", res.index, res.code);
    writelog(LOGFILE, logging);
#endif
*/

    return 1;
}

int evalInstruction(Instruction *inst) {

    int jmp = functionList[inst->index](inst);
    inst->tacts = opcodes[inst->index].tacts;    
    if(inst->execute && inst->delay == PIPE_WRITE && jmp == 1)
        incrementPC();
    inst->execute = 0;
    return jmp;
}

int evalOneCycle(int *tact) {
    //uint16_t opcode;
    int use_inc;
    Instruction instruction = initInstruction();

#ifdef WRITELOG
    sprintf(logging, "\nR0:%o R1:%o R2:%o R3:%o R4:%o R5:%o R6:%o R7:%o",
           registers[0], registers[1], registers[2], registers[3], registers[4],
            registers[5], registers[6], registers[7]);
    writelog(LOGFILE, logging);
#endif
    instruction.execute = 0;
    (*tact) ++;
    fetchMem(&instruction);

    (*tact) ++;

    instruction.execute = 0;
    decode(&instruction);
    if(instruction.index == OP_HALT) return -1;

    //printf("%d %o %s\n", *getRegister(PC_REG), opcode, opcodes[instruction.index].name);

#ifdef WRITELOG
    writelog(LOGFILE, lastInstruction);
#endif

    (*tact)++;
    instruction.execute = 0;
    fetchOperands(&instruction);

    (*tact)++;
    instruction.execute = 0;
    use_inc = evalInstruction(&instruction);

    (*tact)++;
    instruction.execute = 0;
    writeOperands(&instruction);

    timeNop(3);

    return use_inc;
}

int evalPipeline(Instruction *inst) {

    switch(inst->step) {
    case PIPE_FETCH:
        //puts("fetch");
        if(inst->execute) fetchMem(inst);
        inst->delay = PIPE_FETCH_OP;
        if(--(inst->tacts) == 0) nextStep(inst);
        break;
    case PIPE_DECODE:
        //puts("decode");
        if(inst->execute) decode(inst);
        if(--(inst->tacts) == 0) nextStep(inst);
        break;
    case PIPE_FETCH_OP:
        //puts("fetch_operands");
        if(inst->execute) fetchOperands(inst);
        if(--(inst->tacts) == 0) nextStep(inst);
        break;
    case PIPE_EVAL:
        //puts("evaluate");
        if(inst->execute) evalInstruction(inst);
        if(--(inst->tacts) == 0) nextStep(inst);
        break;
    case PIPE_WRITE:
        //puts("write");
        if(inst->execute) writeOperands(inst);
        if(--(inst->tacts) == 0) nextStep(inst);
        break;
    case PIPE_END:
        //puts("end");
        *inst = initInstruction();
        break;
    case PIPE_HALT:
        break;    
    default:
        break;
    }

    return 1;
}

int evalOneTact(int pipeNum) {

    int canFetch = 1, n;
    int evaluated = 0;

    for(n = 0; n < pipeNum; ++n) {
        if(pipes[n].step != pipes[n].delay || pipes[n].step == PIPE_HALT) canFetch = 0;
    }

    for(int n = 0; n < pipeNum; ++n) {
        if(pipes[n].step == PIPE_FETCH && !canFetch)
            continue;
        if(pipes[n].step == PIPE_HALT) continue;

        evalPipeline(pipes+n);
        evaluated = 1;

        // check possibility of read next instruction
        canFetch = canFetch && (pipes[n].step == pipes[n].delay) && (pipes[n].step != PIPE_HALT);
    }

    return evaluated;
}

void resetPipes(void) {
    for(int p = 0; p < PIPE_NUMBER_MAX; ++p) {
        pipes[p]=initInstruction();
    }
}

// Preform initial operations
void prepareProcessor() {    
    initializeFunctions();
    resetFlags();
    resetRegisters();
    resetPipes();
}

void saveState(void) {
    int i;

    storeFlags = flags;
    for(i = 0; i < REG_NUMBER; ++i) { storeRegisters[i] = registers[i]; }
    for(i = 0; i < PIPE_NUMBER_MAX; ++i) { storePipes[i] = pipes[i]; }
}

void restoreState(void) {
    int i;

    flags = storeFlags;
    for(i = 0; i < REG_NUMBER; ++i) { registers[i] = storeRegisters[i]; }
    for(i = 0; i < PIPE_NUMBER_MAX; ++i) { pipes[i] = storePipes[i]; }
}

int evalCode() {
    int tact = 0, increment = 1;

    prepareProcessor();

    while(1) {
        increment = evalOneCycle(&tact);

        if(increment == -1) break; // get HALT instruction

        if(increment)              // brake and jump return 0
            incrementPC();
    }

    return tact;
}

int evalSuperscalar(int pipeNum) {
    if(pipeNum < 1 || pipeNum > PIPE_NUMBER_MAX) return 0;

    int tact = 0;
    prepareProcessor();

    while(evalOneTact(pipeNum)) {
        tact ++;
    }

    return tact;
}

void newProgramm(uint16_t start) {
    resetFlags();
    resetRegisters();
    resetPipes();
    setProgrammStart(start);
}


void printRegisters() {
    printf("\nR0:%o R1:%o R2:%o R3:%o R4:%o R5:%o R6:%o R7:%o\n",
           registers[0], registers[1], registers[2], registers[3], registers[4],
            registers[5], registers[6], registers[7]);
}

int testProcessor2() {
    int tact = 0, i/*, increment = 1*/;

    prepareProcessor();

    // test string(s)
        uint8_t *mem = (uint8_t*) programm_;
        for(i = 20; i < 108; ++i) {
            printf("%c", (char) mem[i]);
        }
        printf("\n");


    printRegisters();
/*
    int k;
    for(k = 0; k < 100; ++k) {
        evalOneTact();*/

    while(evalOneTact(2)) {
        tact++;

        printRegisters();
    }

    printf("\nNumber of tacts: %d\n", tact);
        printf("\n");
        for(i = 20; i < 108; ++i) {
            printf("%c", (char) mem[i]);
        }
        printf("\n");

    return tact;
}



