# Atom2D
Tiny 2D graphics engine in C-language using SDL2+

Usage: `make clean && make all && python ffi.py`  

<img width="810" height="624" alt="image" src="https://github.com/user-attachments/assets/474eacab-a8b7-484d-bb7b-a5b47e4c602c" />


*Early testing*  

# Core Features

Types:  
- Vec2: `typedef struct { float x, y; } Vec2;`
- Rect: `typedef struct { float x, y, w, h; } Rect;`
- Color: `typedef struct { uint8_t r, g, b, a; } Color;`
- Texture Objects
- Font Objects (with font measure helpers)
- Mouse & Keyboard input scancodes handling
- FFI module for Python coupling
- Utilities toolkit (math, geometric, raycast and hitbox logic)

# To do

- Proper `last_error` logic
- Plugin manager
