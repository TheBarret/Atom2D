# Atom2D
Tiny 2D graphics engine in C-language using SDL2+

Usage: `make clean && make all && python ffi.py`  

<img width="810" height="624" alt="image" src="https://cdn.discordapp.com/attachments/272805431745839106/1543652949170065518/image.png?ex=6a95a656&is=6a9454d6&hm=197e513c8690a579eb45623c11b8200ca7d57b0d7c2d015ff40c272be8094c3c&" />


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
