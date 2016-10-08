
#ifndef MEMORY_H
#define MEMORY_H

#include "stdbool.h"
#include "stdint.h"

#define VIDEO_HEIGHT   256
#define VIDEO_WIDTH    256

#define MEMMORY_IVT_SIZE        16384
#define MEMMORY_RAM_SIZE        16384
#define MEMMORY_VIDEO_RAM_SIZE  16384
#define MEMMORY_ROM_SIZE        16384
#define MEMMORY_IO_SIZE         16384

#define REGISTER_NUMBER         6

extern bool memmoryInitialize();

extern const uint8_t* getVideoRom();

extern uint16_t getVideoHeight();
extern uint16_t getVideoWidth();

#endif // MEMORY_H
