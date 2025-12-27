#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

#include "loop.h"
#include "chip.h"
#include "render.h"
#include "input.h"
#include "default.h"

extern bool running;
extern SDL_Window* window;

void null(Input) {}

void loop() {
    const double fps = 1000;
    double dt = 0;
    u64 last = SDL_GetPerformanceCounter();
    init_render();
    chip8_fb_test();
    while (running) {
        /* dt calc */
        u64 now = SDL_GetPerformanceCounter();
        dt = (double)(now - last) / SDL_GetPerformanceCounter();
        last = now;

        for (int i = 0; i < 10; i++) {
            chip8_step();
        }

        Input i = poll_input();

        render();

        null(i);

        double delay = (1.0f / fps - dt);
        if (delay > 0.0) {
            SDL_Delay(delay * 1000);
        } else {
            SDL_Delay(1);
        }
    }
}
