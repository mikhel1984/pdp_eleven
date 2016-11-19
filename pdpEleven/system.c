
#include "system.h"
#include "memory.h"
#include "logo.h"
#include "font.h"
#include "processor.h"

/// Defenition
void loadLogo(void);
void loadFonts(void);


/// Public function

BOOL systemInitialize(void)
{
    if(!memmoryInitialize())
        return FALSE;

    loadLogo();
    loadFonts();
}

void systemDestroy(void)
{
    memmoryDestroy();
}


/// Private function

void loadLogo(void)
{
    memory_ptr romPtr = memoryGetRom();
    int i;

    for(i = 0; i < (logoHeight*logoWidth)/8; i++)
        romPtr[i] = logoData[i];
}

void loadFonts(void)
{
    unsigned int i, j;
    uint8_t r, v;
    int s;
    uint8_t *mem =  getMemory(FONTS_ADR);


    for(i = 0; i < TOTAL_SYMBOL; ++i) {
        for(j = 0; j < CH_HEIGHT; ++j) {
            // reverse bits
            v = r = font[i][j];
            s = sizeof(v)*8 - 1;
            for(v >>= 1; v ; v >>= 1) {
                r <<= 1;
                r |= v & 1;
                s --;
            }
            r <<= s;

            *mem = r;
            mem++;
        }
    }
}
