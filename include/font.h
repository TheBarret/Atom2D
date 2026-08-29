#ifndef FONT_H
#define FONT_H
#include "engine.h"
#include "types.h"

typedef struct Font Font;   // opaque

Font* engine_font_load(Engine* engine, const char* path, int size);
void engine_font_destroy(Font* font);
Texture engine_font_render_text(Engine* engine, Font* font, const char* text, Color c);

#endif
