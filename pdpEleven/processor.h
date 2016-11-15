#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

void prepareProcessor();


extern int evalCode();
extern int evalOneCircle(int *tact);
extern uint16_t *getRegister(uint8_t ind);
extern uint8_t *getMemory(uint16_t ind);

int testProcessor2();





#endif // PROCESSOR_H
