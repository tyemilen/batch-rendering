CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
		-Wcast-align -Wstrict-aliasing -Wformat=2 -Wundef \
		-g -O0 -fstack-protector-strong -D_FORTIFY_SOURCE=2
INCLUDE = -I./include
OUTPUT = ./lib/libyta.a
TARGET = libyta.a

PLATFORM ?=
DEBUG ?=

COMMON_SOURCES = $(filter-out ./src/platform/%.c,$(wildcard ./src/*.c) $(wildcard ./src/**/*.c))

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG
endif

ifeq ($(PLATFORM),win32)
CFLAGS += -DPLATFORM_WINDOWS
PLATFORM_SOURCES = src/platform/win32.c
LIBS += -lopengl32 -lGdi32
endif

ifeq ($(PLATFORM),sdl3)
CFLAGS += $(shell pkg-config --cflags sdl3) -DPLATFORM_SDL3
PLATFORM_SOURCES = src/platform/sdl3.c
endif

SOURCES = $(COMMON_SOURCES) $(PLATFORM_SOURCES)
OBJECTS = $(SOURCES:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) rcs $(OUTPUT) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(OUTPUT)
