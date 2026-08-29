#include "../include/effects.h"
#include "../include/interior.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct CrtEffects {
    SDL_Texture* screenBuffer;
    SDL_Texture* scanlineOverlay;
    SDL_Texture* vignetteTexture;
    int width, height;
};

static SDL_Texture* build_scanlines(SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET, width, height);
    if (!tex) return NULL;
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    SDL_Texture* prev = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 90);
    for (int y = 0; y < height; y += 2) {
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }
    SDL_SetRenderTarget(renderer, prev);
    return tex;
}

// Vignette needs a smooth radial gradient, which render-primitive drawing can't produce cleanly,
// this writes pixels directly.
// SDL_PIXELFORMAT_RGBA8888 is a packed format whose in-memory byte order depends on host endianness,
// so we go through SDL_AllocFormat + SDL_MapRGBA rather than hand-shifting bytes,
// hand-packing here is exactly the kind of thing that silently corrupts colors only on certain platforms.
static SDL_Texture* build_vignette(SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!tex) return NULL;
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if (!fmt) { SDL_DestroyTexture(tex); return NULL; }

    void* pixels;
    int pitch;
    if (SDL_LockTexture(tex, NULL, &pixels, &pitch) != 0) {
        SDL_FreeFormat(fmt);
        SDL_DestroyTexture(tex);
        return NULL;
    }

    float cx = width / 2.0f, cy = height / 2.0f;
    float max_dist = sqrtf(cx * cx + cy * cy);

    for (int y = 0; y < height; y++) {
        uint32_t* row = (uint32_t*)((uint8_t*)pixels + y * pitch);
        for (int x = 0; x < width; x++) {
            float dx = x - cx, dy = y - cy;
            float dist = sqrtf(dx * dx + dy * dy) / max_dist;
            float darken = dist * dist;
            if (darken > 1.0f) darken = 1.0f;
            uint8_t alpha = (uint8_t)(darken * 255.0f);
            row[x] = SDL_MapRGBA(fmt, 0, 0, 0, alpha);
        }
    }

    SDL_UnlockTexture(tex);
    SDL_FreeFormat(fmt);
    return tex;
}

CrtEffects* crt_effects_create(Engine* engine, int width, int height) {
    if (!engine || !engine->renderer) return NULL;

    CrtEffects* crt = (CrtEffects*)calloc(1, sizeof(CrtEffects));
    if (!crt) return NULL;

    SDL_Renderer* renderer = (SDL_Renderer*)engine->renderer;

    crt->screenBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, width, height);
    crt->scanlineOverlay = build_scanlines(renderer, width, height);
    crt->vignetteTexture = build_vignette(renderer, width, height);
    crt->width = width;
    crt->height = height;

    if (!crt->screenBuffer || !crt->scanlineOverlay || !crt->vignetteTexture) {
        fprintf(stderr, "CRT effects setup failed: %s\n", SDL_GetError());
        crt_effects_destroy(crt);
        return NULL;
    }
    return crt;
}

void crt_effects_destroy(CrtEffects* crt) {
    if (!crt) return;
    if (crt->screenBuffer) SDL_DestroyTexture(crt->screenBuffer);
    if (crt->scanlineOverlay) SDL_DestroyTexture(crt->scanlineOverlay);
    if (crt->vignetteTexture) SDL_DestroyTexture(crt->vignetteTexture);
    free(crt);
}

void crt_effects_begin(CrtEffects* crt, Engine* engine) {
    if (!crt || !engine || !engine->renderer) return;
    SDL_SetRenderTarget((SDL_Renderer*)engine->renderer, crt->screenBuffer);
}

void crt_effects_end(CrtEffects* crt, Engine* engine, const Config* cfg, uint32_t elapsed_ms) {
    if (!crt || !engine || !engine->renderer || !cfg) return;
    SDL_Renderer* renderer = (SDL_Renderer*)engine->renderer;

    SDL_SetRenderTarget(renderer, NULL);   // back to the real window

    if (cfg->chromatic_aberration_enabled && cfg->chromatic_aberration_px > 0.0f) {
        // Split-channel offset: draw the buffer three times, once per color
        // channel isolated via color mod, each shifted a few px, additively blended.
        SDL_SetTextureBlendMode(crt->screenBuffer, SDL_BLENDMODE_ADD);
        float off = cfg->chromatic_aberration_px;

        SDL_SetTextureColorMod(crt->screenBuffer, 255, 0, 0);
        SDL_FRect r_dst = { -off, 0, (float)crt->width, (float)crt->height };
        SDL_RenderCopyF(renderer, crt->screenBuffer, NULL, &r_dst);

        SDL_SetTextureColorMod(crt->screenBuffer, 0, 255, 0);
        SDL_FRect g_dst = { 0, 0, (float)crt->width, (float)crt->height };
        SDL_RenderCopyF(renderer, crt->screenBuffer, NULL, &g_dst);

        SDL_SetTextureColorMod(crt->screenBuffer, 0, 0, 255);
        SDL_FRect b_dst = { off, 0, (float)crt->width, (float)crt->height };
        SDL_RenderCopyF(renderer, crt->screenBuffer, NULL, &b_dst);

        SDL_SetTextureColorMod(crt->screenBuffer, 255, 255, 255);
        SDL_SetTextureBlendMode(crt->screenBuffer, SDL_BLENDMODE_BLEND);
    } else {
        SDL_RenderCopy(renderer, crt->screenBuffer, NULL, NULL);
    }

    if (cfg->scanlines_enabled) {
        SDL_SetTextureAlphaMod(crt->scanlineOverlay, (uint8_t)(cfg->scanline_strength * 255.0f));
        SDL_RenderCopy(renderer, crt->scanlineOverlay, NULL, NULL);
    }

    if (cfg->vignette_enabled) {
        SDL_SetTextureAlphaMod(crt->vignetteTexture, (uint8_t)(cfg->vignette_strength * 255.0f));
        SDL_RenderCopy(renderer, crt->vignetteTexture, NULL, NULL);
    }

    if (cfg->roll_bar_enabled) {
        float y = fmodf((elapsed_ms / 1000.0f) * cfg->roll_bar_speed, (float)crt->height);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 15);
        SDL_FRect bar = { 0, y, (float)crt->width, 6.0f };
        SDL_RenderFillRectF(renderer, &bar);
    }

    if (cfg->flicker_enabled) {
        float t = elapsed_ms / 1000.0f;
        float flicker = (sinf(t * 60.0f) * 0.5f + 0.5f) * cfg->flicker_strength;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, (uint8_t)(flicker * 255.0f));
        SDL_RenderFillRect(renderer, NULL);
    }
}
