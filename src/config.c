// config.c
#include "config.h"

Config config_default(void) {
    return (Config){
        .scanlines_enabled = 1, .scanline_strength = 0.35f,
        .vignette_enabled = 1, .vignette_strength = 0.5f,
        .chromatic_aberration_enabled = 0, .chromatic_aberration_px = 1.5f,
        .flicker_enabled = 0, .flicker_strength = 0.02f,
        .roll_bar_enabled = 0, .roll_bar_speed = 40.0f,
    };
}
