#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

extern void prepareProcessor();


extern int evalCode();
extern int evalOneCycle(int *tact);
extern uint16_t *getRegister(uint8_t ind);
extern uint8_t *getMemory(uint16_t ind);
extern void setProgrammStart(uint16_t ind);

char* getLastInstruction();

int testProcessor2();






#endif // PROCESSOR_H
