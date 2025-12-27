#include "chip.h"

Chip8 chip8;

void chip8_fb_test() {
    for (int i = 0; i < 64 * 32; i++) {
        chip8.framebuffer[i] = (i % 2) ? 255 : 0;
    }
}
