#include "../include/utilities.h"

//  Scalar Utilities
float vec2_clamp(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

float vec2_lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

//  Vector Math
Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){ a.x + b.x, a.y + b.y }; }
Vec2 vec2_sub(Vec2 a, Vec2 b) { return (Vec2){ a.x - b.x, a.y - b.y }; }
Vec2 vec2_scale(Vec2 v, float s) { return (Vec2){ v.x * s, v.y * s }; }

float vec2_length_sq(Vec2 v) { return v.x * v.x + v.y * v.y; }
float vec2_length(Vec2 v) { return sqrtf(vec2_length_sq(v)); }

Vec2 vec2_normalize(Vec2 v) {
    float len = vec2_length(v);
    if (len < 1e-6f) return (Vec2){0.0f, 0.0f};
    return (Vec2){v.x / len, v.y / len};
}

float vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
float vec2_cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
float vec2_distance(Vec2 a, Vec2 b) { return vec2_length(vec2_sub(a, b)); }

Vec2 vec2_rotate(Vec2 v, float angle_rad) {
    float cos_a = cosf(angle_rad);
    float sin_a = sinf(angle_rad);
    return (Vec2){
        v.x * cos_a - v.y * sin_a,
        v.x * sin_a + v.y * cos_a
    };
}

//  Geometry & Collision
bool check_aabb_aabb(Rect a, Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

bool check_circle_circle(Circle a, Circle b) {
    float r_sum = a.r + b.r;
    return vec2_length_sq(vec2_sub((Vec2){a.x, a.y}, (Vec2){b.x, b.y})) <= (r_sum * r_sum);
}

bool check_circle_aabb(Circle c, Rect r) {
    float closest_x = vec2_clamp(c.x, r.x, r.x + r.w);
    float closest_y = vec2_clamp(c.y, r.y, r.y + r.h);

    float dx = c.x - closest_x;
    float dy = c.y - closest_y;

    return (dx * dx + dy * dy) <= (c.r * c.r);
}

bool check_point_in_rect(Vec2 p, Rect r) {
    return (p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
}

bool check_point_in_circle(Vec2 p, Circle c) {
    float dx = p.x - c.x;
    float dy = p.y - c.y;
    return (dx * dx + dy * dy) <= (c.r * c.r);
}

//  Matrix 3x3 Transformations
Mat3 mat3_identity(void) {
    Mat3 m = {0};
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f;
    return m;
}

Mat3 mat3_mul(Mat3 a, Mat3 b) {
    Mat3 res = {0};
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            res.m[r][c] = a.m[r][0] * b.m[0][c] +
                          a.m[r][1] * b.m[1][c] +
                          a.m[r][2] * b.m[2][c];
        }
    }
    return res;
}

Mat3 mat3_translate(float tx, float ty) {
    Mat3 m = mat3_identity();
    m.m[0][2] = tx;
    m.m[1][2] = ty;
    return m;
}

Mat3 mat3_rotate(float angle_rad) {
    Mat3 m = mat3_identity();
    float c = cosf(angle_rad);
    float s = sinf(angle_rad);
    m.m[0][0] = c;  m.m[0][1] = -s;
    m.m[1][0] = s;  m.m[1][1] = c;
    return m;
}

Mat3 mat3_scale(float sx, float sy) {
    Mat3 m = mat3_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    return m;
}

Vec2 mat3_transform_vec2(Mat3 m, Vec2 v) {
    return (Vec2){
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2],
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2]
    };
}

// 2D Cross product helper for vectors from points
static float cross_2d(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

// Line-Line Segment Intersection using 2D cross products
bool line_intersects_line(Line2D a, Line2D b, Vec2* out_intersection) {
    Vec2 r = vec2_sub(a.p2, a.p1);
    Vec2 s = vec2_sub(b.p2, b.p1);
    float rxs = cross_2d(r, s);

    if (fabsf(rxs) < 0.00001f) return false; // Parallel or collinear

    Vec2 qp = vec2_sub(b.p1, a.p1);
    float t = cross_2d(qp, s) / rxs;
    float u = cross_2d(qp, r) / rxs;

    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        if (out_intersection) {
            *out_intersection = (Vec2){ a.p1.x + t * r.x, a.p1.y + t * r.y };
        }
        return true;
    }
    return false;
}

// Ray vs Line Segment
RayHit2D raycast_line(Ray2D ray, Line2D line, float max_distance) {
    RayHit2D hit = {0};
    Vec2 seg_vec = vec2_sub(line.p2, line.p1);
    float rxs = cross_2d(ray.dir, seg_vec);

    if (fabsf(rxs) < 0.00001f) return hit;

    Vec2 qp = vec2_sub(line.p1, ray.origin);
    float t = cross_2d(qp, seg_vec) / rxs;
    float u = cross_2d(qp, ray.dir) / rxs;

    if (t >= 0.0f && t <= max_distance && u >= 0.0f && u <= 1.0f) {
        hit.hit = true;
        hit.distance = t;
        hit.point = (Vec2){ ray.origin.x + t * ray.dir.x, ray.origin.y + t * ray.dir.y };

        // Compute face normal perpendicular to segment
        Vec2 perp = { -seg_vec.y, seg_vec.x };
        hit.normal = vec2_normalize(perp);
        if (vec2_dot(hit.normal, ray.dir) > 0.0f) {
            hit.normal = vec2_scale(hit.normal, -1.0f);
        }
    }
    return hit;
}

// Ray vs AABB (Slab Method)
RayHit2D raycast_rect(Ray2D ray, Rect rect, float max_distance) {
    RayHit2D result = {0};

    float inv_dx = (fabsf(ray.dir.x) > 0.00001f) ? 1.0f / ray.dir.x : 1e30f;
    float inv_dy = (fabsf(ray.dir.y) > 0.00001f) ? 1.0f / ray.dir.y : 1e30f;

    float t1 = (rect.x - ray.origin.x) * inv_dx;
    float t2 = (rect.x + rect.w - ray.origin.x) * inv_dx;
    float t3 = (rect.y - ray.origin.y) * inv_dy;
    float t4 = (rect.y + rect.h - ray.origin.y) * inv_dy;

    float tmin = fmaxf(fminf(t1, t2), fminf(t3, t4));
    float tmax = fminf(fmaxf(t1, t2), fmaxf(t3, t4));

    if (tmax < 0.0f || tmin > tmax || tmin > max_distance) {
        return result;
    }

    result.hit = true;
    result.distance = tmin < 0.0f ? tmax : tmin; // Support ray starting inside rect
    result.point = (Vec2){
        ray.origin.x + ray.dir.x * result.distance,
        ray.origin.y + ray.dir.y * result.distance
    };

    // Calculate surface normal
    Vec2 center = { rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f };
    Vec2 p = vec2_sub(result.point, center);
    float dx = fabsf(p.x) / (rect.w * 0.5f);
    float dy = fabsf(p.y) / (rect.h * 0.5f);

    if (dx > dy) {
        result.normal = (Vec2){ p.x > 0 ? 1.0f : -1.0f, 0.0f };
    } else {
        result.normal = (Vec2){ 0.0f, p.y > 0 ? 1.0f : -1.0f };
    }

    return result;
}

// Ray vs Circle
RayHit2D raycast_circle(Ray2D ray, Circle circle, float max_distance) {
    RayHit2D hit = {0};
    Vec2 oc = vec2_sub(ray.origin, (Vec2){circle.x, circle.y});

    float b = vec2_dot(oc, ray.dir);
    float c = vec2_dot(oc, oc) - (circle.r * circle.r);
    float discriminant = b * b - c;

    if (discriminant < 0.0f) return hit;

    float sqrt_d = sqrtf(discriminant);
    float t = -b - sqrt_d;

    if (t < 0.0f) t = -b + sqrt_d; // If ray origin inside circle

    if (t >= 0.0f && t <= max_distance) {
        hit.hit = true;
        hit.distance = t;
        hit.point = (Vec2){ ray.origin.x + t * ray.dir.x, ray.origin.y + t * ray.dir.y };
        hit.normal = vec2_normalize(vec2_sub(hit.point, (Vec2){circle.x, circle.y}));
    }

    return hit;
}

bool line_intersects_rect(Line2D line, Rect rect, Vec2* out_intersection) {
    Vec2 dir = vec2_sub(line.p2, line.p1);
    float len = vec2_length(dir);
    if (len <= 0.00001f) return false;

    Ray2D ray = { line.p1, vec2_scale(dir, 1.0f / len) };
    RayHit2D hit = raycast_rect(ray, rect, len);

    if (hit.hit && out_intersection) {
        *out_intersection = hit.point;
    }
    return hit.hit;
}
