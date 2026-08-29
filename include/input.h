// input.h
#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>
#include "engine.h"

uint8_t engine_key_down(Engine* engine, int scancode);      // held this frame
uint8_t engine_key_pressed(Engine* engine, int scancode);   // went down this frame
uint8_t engine_key_released(Engine* engine, int scancode);  // went up this frame

void engine_get_mouse_pos(Engine* engine, int* x, int* y);
uint8_t engine_mouse_button_down(Engine* engine, int button);

#endif
