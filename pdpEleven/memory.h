
#ifndef MEMORY_H
#define MEMORY_H

#include "stdbool.h"
#include "stdint.h"

#define VIDEO_HEIGHT   256
#define VIDEO_WIDTH    256

#define MEMORY_IVT_SIZE        16384
#define MEMORY_RAM_SIZE        16384
#define MEMORY_VIDEO_RAM_SIZE  16384
#define MEMORY_ROM_SIZE        16384
#define MEMORY_IO_SIZE         16384

#define REGISTER_NUMBER         6

typedef uint8_t* memory_ptr;

bool memmoryInitialize(void);
void memmoryDestroy(void);

memory_ptr memoryGetVideoRom(void);
memory_ptr memoryGetRom(void);
uint8_t* memoryGetPointer(void);

uint16_t getVideoHeight(void);
uint16_t getVideoWidth(void);

#endif // MEMORY_H
