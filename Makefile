CC      = gcc
#CFLAGS  = -shared -fPIC -O2 -Iinclude
CFLAGS = -shared -fPIC -O2 -Wall -Wextra -Iinclude
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SRC 	= src/types.c src/engine.c src/input.c src/draw.c src/texture.c src/font.c src/utilities.c
TARGET  = a2d.so

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
