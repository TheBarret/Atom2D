// texture.c
#include "../include/texture.h"
#include "../include/interior.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

Texture engine_texture_load(Engine* engine, const char* path) {
    Texture tex = {0};  // handle=NULL, width=0, height=0, loaded=0

    if (!engine || !engine->renderer) return tex;

    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Texture load failed (%s): %s\n", path, IMG_GetError());
        return tex;
    }

    SDL_Texture* sdl_tex = SDL_CreateTextureFromSurface((SDL_Renderer*)engine->renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_FreeSurface(surface);  // pixel data copied into the GPU texture already, surface no longer needed

    if (!sdl_tex) {
        fprintf(stderr, "Texture upload failed (%s): %s\n", path, SDL_GetError());
        return tex;
    }

    tex.handle = sdl_tex;
    tex.width = w;
    tex.height = h;
    tex.loaded = 1;
    return tex;
}

void engine_texture_destroy(Texture* tex) {
    if (!tex || !tex->loaded) return;
    SDL_DestroyTexture((SDL_Texture*)tex->handle);
    tex->handle = NULL;
    tex->loaded = 0;
}

void engine_draw_texture(Engine* engine, Texture* tex, Rect dest) {
    if (!engine || !engine->renderer || !tex || !tex->loaded) return;

    SDL_FRect frect = { dest.x, dest.y, dest.w, dest.h };
    SDL_RenderCopyF((SDL_Renderer*)engine->renderer, (SDL_Texture*)tex->handle, NULL, &frect);
}
