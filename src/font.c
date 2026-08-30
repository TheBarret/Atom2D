#include "../include/font.h"
#include "../include/interior.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

struct Font {
    TTF_Font* handle;
    uint8_t valid;
};

Font* engine_font_load(Engine* engine, const char* path, int size) {
    (void)engine;  // signature symmetry with texture/draw calls

    TTF_Font* ttf = TTF_OpenFont(path, size);
    if (!ttf) {
        fprintf(stderr, "Font load failed (%s): %s\n", path, TTF_GetError());
        return NULL;
    }

    Font* font = (Font*)malloc(sizeof(Font));
    if (!font) {
        TTF_CloseFont(ttf);
        return NULL;
    }
    // success
    font->handle = ttf;
    font->valid = 1;
    return font;
}

void engine_font_destroy(Font* font) {
    // contemplate early return
    if (!font || !font->valid) return;
    // de-allocate and zero out
    if (font->handle) {
        TTF_CloseFont(font->handle);
        font->handle = NULL;
        font->valid = 1;
    }
    free(font);
}

Texture engine_font_render_text(Engine* engine, Font* font, const char* text, Color c) {
    Texture tex = {0};

    if (!engine || !engine->renderer || !font || !font->handle || !text) return tex;

    SDL_Color sdl_color = { c.r, c.g, c.b, c.a };
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font->handle, text, sdl_color);
    if (!surface) {
        fprintf(stderr, "Text render failed: %s\n", TTF_GetError());
        return tex;
    }

    SDL_Texture* sdl_tex = SDL_CreateTextureFromSurface((SDL_Renderer*)engine->renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_FreeSurface(surface);

    if (!sdl_tex) {
        fprintf(stderr, "Text texture upload failed: %s\n", SDL_GetError());
        return tex;
    }

    tex.handle = sdl_tex;
    tex.width = w;
    tex.height = h;
    tex.loaded = 1;
    return tex;
}

void engine_font_measure_text(Font* font, const char* text, int* out_w, int* out_h) {
    if (out_w) *out_w = 0;
    if (out_h) *out_h = 0;

    if (!font || !font->valid || !text) return;

    if (TTF_SizeUTF8(font->handle, text, out_w, out_h) != 0) {
        fprintf(stderr, "Text measure failed: %s\n", TTF_GetError());
        if (out_w) *out_w = 0;
        if (out_h) *out_h = 0;
    }
}
