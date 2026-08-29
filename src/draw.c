#include "../include/draw.h"
#include "../include/interior.h"
#include <SDL2/SDL.h>

void engine_draw_rect(Engine* engine, Rect r, Color c, uint8_t filled) {
    if (!engine || !engine->renderer) return;

    SDL_SetRenderDrawColor((SDL_Renderer*)engine->renderer, c.r, c.g, c.b, c.a);

    SDL_FRect frect = { r.x, r.y, r.w, r.h };
    if (filled) {
        SDL_RenderFillRectF((SDL_Renderer*)engine->renderer, &frect);
    } else {
        SDL_RenderDrawRectF((SDL_Renderer*)engine->renderer, &frect);
    }
}

void engine_draw_line(Engine* engine, Vec2 a, Vec2 b, Color c) {
    if (!engine || !engine->renderer) return;

    SDL_SetRenderDrawColor((SDL_Renderer*)engine->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLineF((SDL_Renderer*)engine->renderer, a.x, a.y, b.x, b.y);
}

void engine_draw_point(Engine* engine, Vec2 p, Color c) {
    if (!engine || !engine->renderer) return;

    SDL_SetRenderDrawColor((SDL_Renderer*)engine->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPointF((SDL_Renderer*)engine->renderer, p.x, p.y);
}
