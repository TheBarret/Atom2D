#include "../include/input.h"
#include "../include/interior.h"
#include <SDL2/SDL.h>

uint8_t engine_key_down(Engine* engine, int scancode) {
    if (!engine || scancode < 0 || scancode >= ENGINE_MAX_KEYS) return 0;
    return engine->key_state_curr[scancode];
}

uint8_t engine_key_pressed(Engine* engine, int scancode) {
    if (!engine || scancode < 0 || scancode >= ENGINE_MAX_KEYS) return 0;
    return engine->key_state_curr[scancode] && !engine->key_state_prev[scancode];
}

uint8_t engine_key_released(Engine* engine, int scancode) {
    if (!engine || scancode < 0 || scancode >= ENGINE_MAX_KEYS) return 0;
    return !engine->key_state_curr[scancode] && engine->key_state_prev[scancode];
}

void engine_get_mouse_pos(Engine* engine, int* x, int* y) {
    if (!engine) { if (x) *x = 0; if (y) *y = 0; return; }
    if (x) *x = engine->mouse_x;
    if (y) *y = engine->mouse_y;
}

uint8_t engine_mouse_button_down(Engine* engine, int button) {
    if (!engine) return 0;
    return (engine->mouse_buttons_curr & SDL_BUTTON(button)) != 0;
}
