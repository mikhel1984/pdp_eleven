
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

/*
 *                      MEMMORY MAP
 */

static const uint32_t MEMORY_TOTAL_SIZE = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE +
                      MEMORY_VIDEO_RAM_SIZE + MEMORY_ROM_SIZE + MEMORY_IO_SIZE +
                      REGISTER_NUMBER;

// offset
static const uint32_t OFFSET_IVT       = 0;
static const uint32_t OFFSET_RAM       = MEMORY_IVT_SIZE;
static const uint32_t OFFSET_VIDEO_RAM = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE;
static const uint32_t OFFSET_ROM       = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE + MEMORY_ROM_SIZE;
static const uint32_t OFFSET_IO        = MEMORY_IVT_SIZE + MEMORY_RAM_SIZE + MEMORY_ROM_SIZE +
                                         MEMORY_ROM_SIZE;

typedef uint8_t* memory_ptr;

bool memmoryInitialize(void);
void memmoryDestroy(void);

memory_ptr memoryGetVideoRom(void);
memory_ptr memoryGetRom(void);
uint8_t* memoryGetPointer(void);

uint16_t getVideoHeight(void);
uint16_t getVideoWidth(void);

#endif // MEMORY_H
