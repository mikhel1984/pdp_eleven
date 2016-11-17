#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

#define _C        (1 << 0)
#define _V        (1 << 1)
#define _Z        (1 << 2)
#define _N        (1 << 3)
#define _T        (1 << 4)

extern void prepareProcessor();

extern int evalCode();
extern int evalOneCycle(int *tact);
extern uint16_t *getRegister(uint8_t ind);
extern uint8_t getFlags();
extern uint8_t *getMemory(uint16_t ind);
extern void setProgrammStart(uint16_t ind);

char* getLastInstruction();

int testProcessor2();






#endif // PROCESSOR_H
