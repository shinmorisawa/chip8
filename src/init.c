#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>

#include "init.h"
#include "chip.h"
#include "loop.h"
#include "glad.h"

bool running = true;

SDL_Window* window = NULL;
SDL_GLContext ctx;

void init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[init.c init] init failed\n");
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

    window = SDL_CreateWindow("chip8", 640, 320, SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "[init.c init] window init failed\n");
        return;
    }

    ctx = SDL_GL_CreateContext(window);
    gladLoadGL();
    glEnable(GL_FRAMEBUFFER_SRGB);

    load_rom("roms/1-chip8-logo.ch8");

    loop();

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
