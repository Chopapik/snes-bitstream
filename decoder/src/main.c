/*
 * decoder – minimal SNES ROM (bitstream).
 * Build: make (requires PVSNESLIB_HOME).
 * ROM: snes.sfc – run in Snes9x 
 */

#include <snes.h>

extern char tilfont, palfont;

void main(void)
{
    consoleInit();

    consoleSetTextMapPtr(0x6800);
    consoleSetTextGfxPtr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 16 * 2, &tilfont, &palfont);

    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    consoleDrawText(10, 10, "decoder - snes-bitstream");
    consoleDrawText(10, 12, "Ready.");

    setScreenOn();

    for (;;)
        WaitForVBlank();
}
