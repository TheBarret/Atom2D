// interior.h
#ifndef INTERIOR_H
#define INTERIOR_H
#include "engine.h"

#define ENGINE_MAX_KEYS 512   // SDL scancode range

struct Engine {
    void* window;
    void* renderer;
    Color clear_color;
    uint8_t running;
    uint64_t last_counter;
    uint64_t frequency;
    float delta_time;
    uint32_t target_fps;

    uint8_t key_state_curr[ENGINE_MAX_KEYS];
    uint8_t key_state_prev[ENGINE_MAX_KEYS];

    int mouse_x, mouse_y;
    uint32_t mouse_buttons_curr;
    uint32_t mouse_buttons_prev;

    char last_error[256];
};

#endif
