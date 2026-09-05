# Atom2D
Tiny 2D graphics engine in C-language using SDL2+

Usage: `make clean && make all && python example.py`  

# Core Features

- Basic Vec2: `typedef struct { float x, y; } Vec2;`
- Basic Rect: `typedef struct { float x, y, w, h; } Rect;`
- Basic Color: `typedef struct { uint8_t r, g, b, a; } Color;`
- Texture Objects
- Font Objects (comes with font helper tools)
- Mouse & Keyboard input scancodes handling
- FFI module for Python coupling
- Utilities toolkit (math, geometric, raycast and hitbox logic)

# To do

- Plugin manager
