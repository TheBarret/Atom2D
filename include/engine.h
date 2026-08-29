// engine.h
#ifndef ENGINE_H
#define ENGINE_H
#include "types.h"
#include <stdint.h>

typedef struct Engine Engine;   // opaque

Engine* engine_create(const char* title, int width, int height, uint32_t target_fps);
void engine_destroy(Engine* engine);

void engine_begin_frame(Engine* engine);
void engine_clear(Engine* engine);
void engine_end_frame(Engine* engine);

float engine_get_delta_time(Engine* engine);
uint32_t engine_get_fps(Engine* engine);
uint8_t engine_is_running(Engine* engine);     // interior branch logic
void engine_set_clear_color(Engine* engine, Color c);

#endif
