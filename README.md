# Atom2D
Tiny 2D graphics engine in C-language using SDL2+

Usage: `make clean && make all && python ffi.py`  

<img width="810" height="624" alt="image" src="https://github.com/user-attachments/assets/5ffb9011-981c-40e0-8db5-63ca358792f7" />  

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
