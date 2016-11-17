#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

// clear and initialize data
extern void prepareProcessor();

// evaluate programm (1 pipeline)
extern int evalCode();
// evaluate one group of instructions (from fetch to write)
extern int evalOneCycle(int *tact);
// return register state
extern uint16_t *getRegister(uint8_t ind);
// pointer for given index
extern uint8_t *getMemory(uint16_t ind);
// set first instruction of programm
extern void setProgrammStart(uint16_t ind);

// evaluate programm with various number of pipelines
extern int evalSuperscalar(int pipeNum);
// evaluate one tact of all pipelines
extern int evalOneTact(int pipeNum);

// save current state of processor
extern void saveState(void);
// restore last saved processor state
extern void restoreState(void);

extern void newProgramm(uint16_t start);

// last instruction after decoding
char* getLastInstruction();

int testProcessor2();






#endif // PROCESSOR_H
