#ifndef PLATFORM_SDL3_H_
#define PLATFORM_SDL3_H_

#include <SDL3/SDL_video.h>

#include "core.h"

int platform_init(Window *window);
void platform_draw(void);

void platform_poll_event(Event*);
Image *platform_load_image(const char* filename);

#endif // PLATFORM_SDL3_H_
