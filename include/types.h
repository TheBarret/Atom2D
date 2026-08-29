// types.h
#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef struct { float x, y; } Vec2;
typedef struct { float x, y, w, h; } Rect;
typedef struct { uint8_t r, g, b, a; } Color;

typedef struct {
    void* handle;
    int32_t width;
    int32_t height;
    uint8_t loaded;
} Texture;

Vec2 vec2(float x, float y);
Rect rect(float x, float y, float w, float h);
Color color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

//void texture_free(Texture* tex); // moved to texture.h

#endif
