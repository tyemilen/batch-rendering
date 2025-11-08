CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
         -Wcast-align -Wstrict-aliasing -Wformat=2 -Wundef \
         -g -O0 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -mconsole
INCLUDE = -I./include -I./third_party
LIBS =
OUTPUT = ./build/libyta
TARGET = libyta

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
CFLAGS += $(shell pkg-config --cflags --libs sdl3) -DPLATFORM_SDL3
LIBS += -lSDL3_image
PLATFORM_SOURCES = src/platform/sdl3.c
endif

SOURCES = $(COMMON_SOURCES) $(PLATFORM_SOURCES)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) $(CFLAGS) -o $(OUTPUT) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run:
	$(OUTPUT)

clean:
	rm -f $(OBJECTS) $(OUTPUT)
