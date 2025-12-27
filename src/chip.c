#include <SDL3/SDL_timer.h>
#include <stdio.h>
#include "chip.h"

Chip8 chip8;

extern bool running;

static u64 last_timer_tick = 0;

void chip8_fb_test() {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            int k = i * 64 + j;
            chip8.framebuffer[k] = ((i + j) & 1) ? 255 : 0;
        }
    }
}

void timer_update() {
    u64 now = SDL_GetTicks();
    if (now - last_timer_tick >= 1000 / 60) {
        if (chip8.d_timer > 0) chip8.d_timer--;
        if (chip8.s_timer > 0) {
            chip8.s_timer--;
            /* TODO: implement beep */
        }
        last_timer_tick = now;
    }
}

void load_rom(const char* file_name) {
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        perror("fopen");
        return;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    if (size > (4096 - 0x200)) {
        fprintf(stderr, "[chip.c load_rom] rom too big\n");
    }

    int i = fread(&chip8.memory[0x200], 1, size, f);
    if (!i) {
        return;
    }
    fclose(f);

    chip8.PC = 0x200;
}

void chip8_step() {
    u16 opcode = chip8.memory[chip8.PC] << 8 | chip8.memory[chip8.PC + 1];
    chip8.PC += 2;

    fprintf(stdout, "[chip.c chip8_step] opcode: %04X, PC=%04X\n", opcode, chip8.PC);

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0xE0:
                    for (int i = 0; i < 64 * 32; i++) {
                        chip8.framebuffer[i] = 0;
                    }
                    break;
                case 0xEE:
                    chip8.PC = chip8.stack[--chip8.SP];
                    break;
            }
            break;
        case 0x1000:
            chip8.PC = opcode & 0xFFF;
            break;
        case 0x2000:
            chip8.stack[chip8.SP++] = chip8.PC;
            chip8.PC = opcode & 0xFFF;
            break;
        case 0x3000:
            if (chip8.V[(opcode & 0xF00) >> 8] == (opcode & 0xFF)) {
                chip8.PC += 2;
            }
            break;
        case 0x4000:
            if (chip8.V[(opcode & 0xF00) >> 8] != (opcode & 0xFF)) {
                chip8.PC += 2;
            }
            break;
        case 0x5000:
            if ((opcode & 0xF) == 0 && chip8.V[(opcode & 0xF00) >> 8] == chip8.V[((opcode & 0xF0) >> 4)]) {
                chip8.PC += 2;
            }
            break;
        case 0x6000:
            chip8.V[(opcode & 0xF00) >> 8] = (opcode & 0xFF);
            break;
        case 0x7000:
            chip8.V[(opcode & 0xF00) >> 8] += (opcode & 0xFF);
            break;
        case 0x8000:
            switch (opcode & 0xF) {
                case 0x0:
                    chip8.V[(opcode & 0xF00) >> 8] = chip8.V[(opcode & 0xF0) >> 4];
                    break;
                case 0x1:
                    chip8.V[(opcode & 0xF00) >> 8] |= chip8.V[(opcode & 0xF0) >> 4];
                    break;
                case 0x2:
                    chip8.V[(opcode & 0xF00) >> 8] &= chip8.V[(opcode & 0xF0) >> 4];
                    break;
                case 0x3:
                    chip8.V[(opcode & 0xF00) >> 8] ^= chip8.V[(opcode & 0xF0) >> 4];
                    break;
                case 0x4:
                    u16 sum = chip8.V[(opcode & 0xF00) >> 8] + chip8.V[(opcode & 0xF0) >> 4];
                    chip8.V[(opcode & 0xF00) >> 8] = sum & 0xFF;
                    chip8.V[0xF] = sum > 0xFF;
                    break;
                case 0x5:
                    chip8.V[0xF] = chip8.V[(opcode & 0xF00) >> 8] >= chip8.V[(opcode & 0xF0) >> 4];
                    chip8.V[(opcode & 0xF00) >> 8] -= chip8.V[(opcode & 0xF0) >> 4];
                    break;
                case 0x6:
                    chip8.V[0xF] = chip8.V[(opcode & 0xF00) >> 8] & 0x1;
                    chip8.V[(opcode & 0xF00) >> 8] >>= 1;
                    break;
                case 0x7:
                    chip8.V[0xF] = chip8.V[(opcode & 0xF0) >> 8] >= chip8.V[(opcode & 0xF00) >> 4];
                    chip8.V[(opcode & 0xF00) >> 8] = chip8.V[(opcode & 0xF0) >> 4] - chip8.V[(opcode & 0xF00) >> 8];
                    break;
                case 0x8:
                    break;
                case 0x9:
                    break;
                case 0xA:
                    break;
                case 0xB:
                    break;
                case 0xC:
                    break;
                case 0xD:
                    break;
                case 0xE:
                    chip8.V[0xF] = (chip8.V[(opcode & 0xF00) >> 8] & 0x80) >> 7;
                    chip8.V[(opcode & 0xF00) >> 8] <<= 1;
                    break;
                case 0xF:
                    break;
            }
            break;
        case 0x9000:
            if ((opcode & 0xF) == 0 && chip8.V[(opcode & 0xF00) >> 8] != chip8.V[((opcode & 0xF0) >> 4)]) {
                chip8.PC += 2;
            }
            break;
        case 0xA000:
            chip8.I = opcode & 0xFFF;
            break;
        case 0xB000:
            chip8.PC = (opcode & 0xFFF) + chip8.V[0];
            break;
        case 0xC000:
            break;
        case 0xD000:
            chip8.V[0xF] = 0;
            for (int row = 0; row < (opcode & 0xF); row++) {
                u8 sprite_byte = chip8.memory[chip8.I + row];

                for (int bit = 0; bit < 8; bit++) {
                    u8 sprite_pixel = (sprite_byte >> (7 - bit)) & 1;

                    if (sprite_pixel == 1) {
                        u8 x = (chip8.V[(opcode & 0xF00) >> 8] + bit) % 64;
                        u8 y = (chip8.V[(opcode & 0xF0) >> 4] + row) % 32;

                        u32 index = y * 64 + x;

                        if (chip8.framebuffer[index] == 255) {
                            chip8.V[0xF] = 1;
                        }

                        chip8.framebuffer[index] ^= 255;
                    }
                }
            }
            break;
        case 0xE000:
            break;
        case 0xF000:
            switch (opcode & 0xFF) {
                case 0x1E:
                    chip8.I += chip8.V[(opcode & 0xF00) >> 8];
                    break;
                case 0x55:
                    for (int i = 0; i <= (opcode & 0xF00) >> 8; i++) {
                        chip8.memory[chip8.I + i] = chip8.V[i];
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= (opcode & 0xF00) >> 8; i++) {
                        chip8.V[i] = chip8.memory[chip8.I + i];
                    }
                    break;
            }
            break;
        default:
            printf("[chip.c chip8_step] unknown opcode: %04X at PC=%04X\n", opcode, chip8.PC - 2);
            break;
    }
}
