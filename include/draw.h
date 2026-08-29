#ifndef DRAW_H
#define DRAW_H
#include "engine.h"
#include "types.h"

void engine_draw_rect(Engine* engine, Rect r, Color c, uint8_t filled);
void engine_draw_line(Engine* engine, Vec2 a, Vec2 b, Color c);
void engine_draw_point(Engine* engine, Vec2 p, Color c);

#endif
