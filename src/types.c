#include "../include/types.h"
#include <stdlib.h>
#include <stdbool.h>

Vec2 vec2(float x, float y) {
    return (Vec2){x, y};
}

Rect rect(float x, float y, float w, float h) {
    return (Rect){x, y, w, h};
}

Color color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Color){r, g, b, a};
}

/*
   //moved to texture.c
    void texture_free(Texture* tex) {
        if (tex && tex->loaded) {
            // SDL will handle this later
            tex->loaded = false;
            tex->handle = 0;
        }
    }
*/
