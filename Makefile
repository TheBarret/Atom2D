CC      = gcc
CFLAGS  = -shared -fPIC -O2 -Iinclude
LDFLAGS = -lSDL2
SRC 	= src/types.c src/engine.c src/input.c src/draw.c src/texture.c src/font.c src/config.c src/effects.c
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
TARGET  = a2d.so

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
