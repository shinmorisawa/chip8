#pragma once

typedef struct Input {
    bool lm, rm;
    bool keys[256];
    int mx, my;
} Input;

Input poll_input();
