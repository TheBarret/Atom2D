#include "interior.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Engine* engine_create(const char* title, int width, int height, uint32_t target_fps) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "SDL Init Failed: %s\n", SDL_GetError());
        return NULL;
    }

    // load image module
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        fprintf(stderr, "IMG_Init Failed: %s\n", IMG_GetError());
        SDL_Quit();
        return NULL;
    }

    // load font module
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Failed: %s\n", TTF_GetError());
        SDL_Quit();
        return NULL;
    }

    // load engine
    Engine* engine = (Engine*)calloc(1, sizeof(Engine));
    if (!engine) return NULL;

    // create window handle
    SDL_Window* win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        width, height, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        free(engine);
        SDL_Quit();
        return NULL;
    }

    // create renderer
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        free(engine);
        SDL_Quit();
        return NULL;
    }

    // create interior plate
    engine->window = win;
    engine->renderer = ren;
    engine->clear_color = color(90, 90, 90, 255);
    engine->running = 1;

    // Setup high-resolution timers
    engine->frequency = SDL_GetPerformanceFrequency();
    engine->last_counter = SDL_GetPerformanceCounter();
    engine->delta_time = 0.0f;
    engine->target_fps = target_fps;

    return engine;
}

void engine_clear(Engine* engine) {
    if (!engine || !engine->renderer) return;

    SDL_SetRenderDrawColor(
        (SDL_Renderer*)engine->renderer,
        engine->clear_color.r,
        engine->clear_color.g,
        engine->clear_color.b,
        engine->clear_color.a
    );
    SDL_RenderClear((SDL_Renderer*)engine->renderer);
}

void engine_destroy(Engine* engine) {
    if (!engine) return;

    if (engine->renderer) {
        SDL_DestroyRenderer((SDL_Renderer*)engine->renderer);
    }
    if (engine->window) {
        SDL_DestroyWindow((SDL_Window*)engine->window);
    }
    // demod modules
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    // de-allocate engine
    free(engine);
}

void engine_begin_frame(Engine* engine) {
    if (!engine) return;

    uint64_t current_counter = SDL_GetPerformanceCounter();
    uint64_t counter_elapsed = current_counter - engine->last_counter;
    engine->delta_time = (float)counter_elapsed / (float)engine->frequency;
    engine->last_counter = current_counter;

    // Snapshot last frame's input before this frame overwrites it
    memcpy(engine->key_state_prev, engine->key_state_curr, sizeof(engine->key_state_curr));
    engine->mouse_buttons_prev = engine->mouse_buttons_curr;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            engine->running = 0;
        }
    }

    int numkeys = 0;
    const uint8_t* sdl_keys = SDL_GetKeyboardState(&numkeys);
    int copy_count = numkeys < ENGINE_MAX_KEYS ? numkeys : ENGINE_MAX_KEYS;
    memcpy(engine->key_state_curr, sdl_keys, copy_count);

    engine->mouse_buttons_curr = SDL_GetMouseState(&engine->mouse_x, &engine->mouse_y);
}

void engine_end_frame(Engine* engine) {
    if (!engine) return;

    // Present the rendered buffer
    SDL_RenderPresent((SDL_Renderer*)engine->renderer);

    // Frame Rate Limiter (Hybrid Sleep + Spin)
    if (engine->target_fps > 0) {
        float target_frame_time = 1.0f / (float)engine->target_fps;

        while (1) {
            uint64_t now = SDL_GetPerformanceCounter();
            float frame_elapsed = (float)(now - engine->last_counter) / (float)engine->frequency;

            if (frame_elapsed >= target_frame_time) {
                break;
            }

            // Sleep for the majority of remaining time to free CPU resources
            float time_remaining = target_frame_time - frame_elapsed;
            if (time_remaining > 0.002f) { // Only sleep if > 2ms remaining
                SDL_Delay((uint32_t)((time_remaining - 0.001f) * 1000.0f));
            }
        }
    }
}

float engine_get_delta_time(Engine* engine) {
    return engine ? engine->delta_time : 0.0f;
}

uint32_t engine_get_fps(Engine* engine) {
    if (!engine || engine->delta_time <= 0.0f) return 0;
    return (uint32_t)(1.0f / engine->delta_time);
}

uint8_t engine_is_running(Engine* engine) {
    return engine ? engine->running : 0;
}

void engine_set_clear_color(Engine* engine, Color c) {
    if (!engine) return;
    engine->clear_color = c;
}
