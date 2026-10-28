CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

SRC = $(wildcard src/*.c)
TARGET = chip8

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
