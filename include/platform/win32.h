#ifndef PLATFORM_WIN32_H_
#define PLATFORM_WIN32_H_

#include "core.h"

int platform_init(Window *window);
void platform_draw(void);

void platform_poll_event(Event *);
Image *platform_load_image(const char *filename);

void platform_get_mouse(Mouse *mouse);

void *platform_gl_proc(const char *);

#endif // PLATFORM_WIN32_H_
