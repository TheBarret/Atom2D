# Atom2D
Tiny 2D graphics engine in C-language using SDL2+

Usage: `make clean && make all && python example.py`  

<img width="810" height="624" alt="image" src="https://cdn.discordapp.com/attachments/1525976686456344596/1543688384042762270/image.png?ex=6a95c757&is=6a9475d7&hm=93db7938c5b858510e678d1d3f5e0b45ff9488b6631a5108c226b66f7cd078a1&" />


*Early testing*  

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
