
#include "memory.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

/*
 *                      MEMMORY MAP
 */

static const uint32_t MEMMORY_TOTAL_SIZE = MEMMORY_IVT_SIZE + MEMMORY_RAM_SIZE +
                      MEMMORY_VIDEO_RAM_SIZE + MEMMORY_ROM_SIZE + MEMMORY_IO_SIZE +
                      REGISTER_NUMBER;

// offset
static const uint32_t OFFSET_IVT       = 0;
static const uint32_t OFFSET_RAM       = MEMMORY_IVT_SIZE;
static const uint32_t OFFSET_VIDEO_RAM = MEMMORY_IVT_SIZE + MEMMORY_RAM_SIZE;
static const uint32_t OFFSET_ROM       = MEMMORY_IVT_SIZE + MEMMORY_RAM_SIZE + MEMMORY_ROM_SIZE;
static const uint32_t OFFSET_IO        = MEMMORY_IVT_SIZE + MEMMORY_RAM_SIZE + MEMMORY_ROM_SIZE +
                                         MEMMORY_ROM_SIZE;

uint8_t* buffer = NULL;

bool memmoryInitialize()
{
    buffer = (uint8_t*)malloc(MEMMORY_TOTAL_SIZE);
    memset(buffer, 0, MEMMORY_TOTAL_SIZE);

    return (buffer) ? true : false;
}

const uint8_t* getVideoRom()
{
    return (buffer + OFFSET_VIDEO_RAM);
}

const uint8_t* getRom()
{
    return buffer + OFFSET_ROM;
}

const uint8_t* getMemoryBuf()
{
    return buffer;
}
