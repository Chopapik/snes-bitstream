#define MAX_TEXT 32
#include <snes.h>

static unsigned char decoded[MAX_TEXT + 1];
static unsigned char current_byte = 0;
static unsigned int cursor = 0;

static void clear_terminal() {
    int i;
    for (i = 0; i < MAX_TEXT + 1; i++) {
        decoded[i] = 0;
    }
}
static char get_buttons() {
    unsigned short pad = padsCurrent(0);
    static unsigned short last_pad;
    static unsigned char building_byte = 0;
    static unsigned char finished_byte = 0;

    if ((pad & KEY_UP) && !(last_pad & KEY_UP)) {
        finished_byte = building_byte;
        building_byte = 0;
    }
    last_pad = pad;

    if (pad & KEY_A) {
        building_byte |= 1 << 0;
    }
    if (pad & KEY_B) {
        building_byte |= 1 << 1;
    }
    if (pad & KEY_X) {
        building_byte |= 1 << 2;
    }
    if (pad & KEY_Y) {
        building_byte |= 1 << 3;
    }
    if (pad & KEY_L) {
        building_byte |= 1 << 4;
    }
    if (pad & KEY_R) {
        building_byte |= 1 << 5;
    }
    if (pad & KEY_START) {
        building_byte |= 1 << 6;
    }
    if (pad & KEY_SELECT) {
        building_byte |= 1 << 7;
    }
    return finished_byte;
}

static void push_char(unsigned char c) {
    if (cursor < MAX_TEXT) {
        decoded[cursor] = c;
        cursor = cursor + 1;
        decoded[cursor] = 0;
    } else {
        int i;
        for (i = 0; i < MAX_TEXT - 1; i++) {
            decoded[i] = decoded[i + 1];
        }
        
        decoded[MAX_TEXT - 1] = c;
        decoded[MAX_TEXT] = 0;
    }
}

static void draw_terminal(void) {
    consoleDrawText(3, 4,  "*** SNES TERMINAL ***");
    consoleDrawText(3, 6,  "Protocol: 8-bit parallel + UP");
    consoleDrawText(3, 8,  "Waiting for data...");
    consoleDrawText(3, 12, ">");
}
    
int main(void) {
    clear_terminal();
   while (1) {
        WaitForVBlank();
        unsigned char final_char = get_buttons();
        if (final_char != 0) {
            push_char(final_char);
        }
        consoleDrawText(3, 12, "> %s", decoded);
    }
    return 0;
    }

