
#ifndef MEMORY_H
#define MEMORY_H

#include "stdbool.h"
#include "stdint.h"

extern bool memmoryInitialize();

extern const uint8_t* getVideoRom();

extern uint16_t getVideoHeight();
extern uint16_t getVideoWidth();

#endif // MEMORY_H
