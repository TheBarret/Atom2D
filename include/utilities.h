#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <stdbool.h>

#define A2D_PI 3.14159265358979323846f
#define A2D_DEG2RAD(deg) ((deg) * (A2D_PI / 180.0f))
#define A2D_RAD2DEG(rad) ((rad) * (180.0f / A2D_PI))

// Core Types
typedef struct { float x, y; } Vec2;
typedef struct { float x, y, w, h; } Rect;
typedef struct { float x, y, r; } Circle;

// 3x3 Matrix for 2D Transformations (Translation, Rotation, Scale)
typedef struct {
    float m[3][3];
} Mat3;

//  Scalar Utilities
float vec2_clamp(float val, float min, float max);
float vec2_lerp(float a, float b, float t);

//  Vector Operations
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_scale(Vec2 v, float s);
float vec2_length(Vec2 v);
float vec2_length_sq(Vec2 v);
Vec2 vec2_normalize(Vec2 v);
float vec2_dot(Vec2 a, Vec2 b);
float vec2_cross(Vec2 a, Vec2 b); // Returns 2D scalar cross product z-component
float vec2_distance(Vec2 a, Vec2 b);
Vec2 vec2_rotate(Vec2 v, float angle_rad);

//  Geometry & Collision
bool check_aabb_aabb(Rect a, Rect b);
bool check_circle_circle(Circle a, Circle b);
bool check_circle_aabb(Circle c, Rect r);
bool check_point_in_rect(Vec2 p, Rect r);
bool check_point_in_circle(Vec2 p, Circle c);

//  Matrix 3x3 (Transforms & Projection)
Mat3 mat3_identity(void);
Mat3 mat3_mul(Mat3 a, Mat3 b);
Mat3 mat3_translate(float tx, float ty);
Mat3 mat3_rotate(float angle_rad);
Mat3 mat3_scale(float sx, float sy);
Vec2 mat3_transform_vec2(Mat3 m, Vec2 v);

// Ray and Line Segment
typedef struct { Vec2 origin; Vec2 dir; } Ray2D;
typedef struct { Vec2 p1; Vec2 p2; } Line2D;

typedef struct {
    bool hit;
    Vec2 point;
    Vec2 normal;
    float distance;
} RayHit2D;

//  Line Intersections
bool line_intersects_line(Line2D a, Line2D b, Vec2* out_intersection);
bool line_intersects_rect(Line2D line, Rect rect, Vec2* out_intersection);

//  Raycasting
RayHit2D raycast_line(Ray2D ray, Line2D line, float max_distance);
RayHit2D raycast_rect(Ray2D ray, Rect rect, float max_distance);
RayHit2D raycast_circle(Ray2D ray, Circle circle, float max_distance);
#endif
