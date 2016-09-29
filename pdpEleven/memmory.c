
#include "memmory.h"
#include "stdint.h"

/*                     MEMMORY MAP
 *
 *
 */

static const uint32_t memmoryIvtSize      = 16384;
static const uint32_t memmoryRamSize      = 16384;
static const uint32_t memmoryVideoRamSize = 16384;
static const uint32_t memmoryRomSize      = 16384;
static const uint32_t memmoryIOSize       = 16384;

static const uint8_t  registerNumber = 6;

static const uint32_t memmoryTotalSize = memmoryIvtSize + memmoryRamSize +
        memmoryVideoRamSize + memmoryRomSize + memmoryIOSize +
        registerNumber;

// offset
static const uint32_t offsetIvt = 0;
static const uint32_t offsetRam = memmoryIvtSize;
static const uint32_t offsetVideoRam = offsetRam + memmoryRamSize;
static const uint32_t offsetRom = offsetVideoRam + memmoryRomSize;
static const uint32_t offsetIO = offsetRom + memmoryRomSize;

uint8_t* buffer = NULL;

bool memmoryInitialize()
{
    buffer = (uint8_t*)malloc(memmorySize);
    memset(buffer, memmorySize, 0);

    return (buff) ? true : false;
}
