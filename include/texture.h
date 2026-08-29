// texture.h
#ifndef TEXTURE_H
#define TEXTURE_H
#include "engine.h"
#include "types.h"

Texture engine_texture_load(Engine* engine, const char* path);
void engine_texture_destroy(Texture* tex);
void engine_draw_texture(Engine* engine, Texture* tex, Rect dest);

#endif
