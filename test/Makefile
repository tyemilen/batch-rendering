CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
		-Wcast-align -Wstrict-aliasing -Wformat=2 -Wundef \
		-g -O1 -fstack-protector-strong -D_FORTIFY_SOURCE=2 \

INCLUDE = -I../include
OUTPUT = ./yta_test.out
COMMON_SOURCES = $(wildcard *.c)

LIBS = -L../lib -lyta $(shell pkg-config --cflags --libs sdl3 sdl3-image) -lm

all:
	$(CC) $(CFLAGS) $(COMMON_SOURCES) $(LIBS) $(INCLUDE) -o $(OUTPUT)