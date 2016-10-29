
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "common.h"

extern uint16_t arrayInit(void);
extern uint16_t* arrayInstance(void);
extern void arrayPush(uint16_t value);
extern uint16_t arraySize(void);

extern uint16_t arrayCurrIndex();
extern void arraySetValue(uint16_t index, uint16_t value);

extern uint16_t arrayPrint(void);

#endif // ARRAYLIST_H
