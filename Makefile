CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
		 -Wcast-align -Wstrict-aliasing -Wformat=2 -Wundef \
		 -g -O0 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -DDEBUG
SOURCES = $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
INCLUDE = -I./include
LIBS = -lOpenGL32 -lglew32
LIB_SDL = $(shell pkg-config --cflags --libs sdl3)

OBJECTS = $(SOURCES:.c=.o)

OUTPUT = ./build/batch

TARGET = batch

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) $(CFLAGS) -o $(OUTPUT) -mconsole $(LIBS) $(LIB_SDL)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $(LIB_SDL) -c $< -o $@

run:
	$(OUTPUT)

clean:
	rm -f $(OBJECTS) $(OUTPUT)
