#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include "input.h"

extern bool running;

static Input input;

Input poll_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT: running = false; break;
            case SDL_EVENT_MOUSE_MOTION:
                input.mx = e.motion.x;
                input.my = e.motion.y;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (e.button.button == SDL_BUTTON_LEFT)  input.lm = true;
                if (e.button.button == SDL_BUTTON_RIGHT) input.rm = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (e.button.button == SDL_BUTTON_LEFT)  input.lm = false;
                if (e.button.button == SDL_BUTTON_RIGHT) input.rm = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.scancode == SDL_SCANCODE_ESCAPE) { running = false; break; }
                input.keys[e.key.scancode] = true;
                break;
            case SDL_EVENT_KEY_UP:
                input.keys[e.key.scancode] = false;
                break;
        }
    }

    return input;
}
