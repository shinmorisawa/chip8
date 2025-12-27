#pragma once

#include "default.h"

typedef struct Chip8 {
    u8  V[16];
    u16 I;
    u16 PC;
    u8  memory[4096];
    u8  d_timer;
    u8  s_timer;
    u16 stack[16];
    u8  SP;
    u8  framebuffer[64 * 32];
    u8  keys[16];
} Chip8;

void chip8_fb_test();
void load_rom(const char* rom);
void chip8_step();
