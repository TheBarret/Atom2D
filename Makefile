CC      = gcc
CFLAGS  = -shared -fPIC -O2 -Iinclude
LDFLAGS = -lSDL2
SRC 	= src/types.c src/engine.c src/input.c src/draw.c
TARGET  = a2d.so

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
