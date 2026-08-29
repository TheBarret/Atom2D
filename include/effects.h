#ifndef EFFECTS_H
#define EFFECTS_H
#include <stdint.h>
#include "engine.h"
#include "config.h"

typedef struct CrtEffects CrtEffects;   // opaque

CrtEffects* crt_effects_create(Engine* engine, int width, int height);
void crt_effects_destroy(CrtEffects* crt);

// Redirects subsequent draw calls into the CRT's offscreen buffer instead of the window.
void crt_effects_begin(CrtEffects* crt, Engine* engine);

// Stops redirecting, composites the buffer onto the window with the configured post effects.
void crt_effects_end(CrtEffects* crt, Engine* engine, const Config* cfg, uint32_t elapsed_ms);

#endif
