#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>

typedef struct {
    uint8_t scanlines_enabled;
    float scanline_strength;        // 0.0 - 1.0

    uint8_t vignette_enabled;
    float vignette_strength;        // 0.0 - 1.0

    uint8_t chromatic_aberration_enabled;
    float chromatic_aberration_px;  // channel offset in pixels

    uint8_t flicker_enabled;
    float flicker_strength;         // 0.0 - 1.0

    uint8_t roll_bar_enabled;
    float roll_bar_speed;           // pixels/sec
} Config;

Config config_default(void);

#endif
