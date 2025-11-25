#include "core.h"
#include "ygl.h"
#include "log.h"

#ifdef PLATFORM_SDL3
#include "platform/sdl3.h"
#endif
#ifdef PLATFORM_WIN32
#include "platform/win32.h"
#endif

void core_init_gl(Window window) {
	ygl_init((GLProcLoader)platform_gl_proc);

	glViewport(0, 0, window.width, window.height);
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	LOG_INFO("Done with GL");
}

Window core_create_window(char *title, int width, int height) {
	Window window = {.title = title, .width = width, .height = height};
	window.mouse = (Mouse){0};

	platform_init(&window);
	core_get_mouse(&window.mouse);

	return window;
}

void core_draw(void) { platform_draw(); }

int core_poll_event(Event *event) {
	platform_poll_event(event);
	return event->type;
}

int core_get_key(int k) {
	return platform_get_key(k);
}

Image *core_load_image(const char *filename) {
	return platform_load_image(filename);
}

void core_get_mouse(Mouse *mouse) { platform_get_mouse(mouse); }
