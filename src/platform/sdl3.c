#include "platform/sdl3.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "ygl.h"

#include "log.h"

typedef struct {
	SDL_Window *window;
	SDL_GLContext glContext;

	SDL_Cursor *cursor;
} PlatformData;

PlatformData platform = {0};

int platform_init(Window *window) {
	if (!SDL_Init(SDL_INIT_VIDEO)) return 0;
	platform.window = SDL_CreateWindow(window->title, window->width,
									   window->height, SDL_WINDOW_OPENGL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	platform.glContext = SDL_GL_CreateContext(platform.window);
	SDL_GL_MakeCurrent(platform.window, platform.glContext);
	SDL_GL_SetSwapInterval(1);

	window->gl_version_str = "#version 330";

	return 1;
}

void platform_draw(void) { SDL_GL_SwapWindow(platform.window); }

void platform_poll_event(Event *event) {
	SDL_Event sdl_event;
	SDL_PollEvent(&sdl_event);

	event->data1 = 0;
	event->data2 = 0;

	switch (sdl_event.type) {
	case SDL_EVENT_QUIT:
		event->type = EVENT_EXIT;
		break;
	default:
		event->type = EVENT_NONE;
		break;
	}
}

static int convert_pixel_format(int format) {
	switch (format) {
	case SDL_PIXELFORMAT_RGBA32:
		return GL_RGBA;
	case SDL_PIXELFORMAT_ARGB32:
		return GL_BGRA;
	case SDL_PIXELFORMAT_RGB24:
		return GL_RGB;
	case SDL_PIXELFORMAT_BGR24:
		return GL_BGR;
	default:
		return -1;
	}
}

Image *platform_load_image(const char *filename) {
	Image *image = malloc(sizeof(Image));

	SDL_Surface *surface = IMG_Load(filename);
	if (!surface) {
		free(image);
		return 0;
	}

	image->format = convert_pixel_format(surface->format);
	image->width = surface->w;
	image->height = surface->h;

	const SDL_PixelFormatDetails *details =
		SDL_GetPixelFormatDetails(surface->format);
	size_t size = surface->w * surface->h * details->bytes_per_pixel;

	image->pixels = malloc(size);
	memcpy(image->pixels, surface->pixels, size);

	SDL_DestroySurface(surface);

	return image;
}

void platform_get_mouse(Mouse *mouse) {
	SDL_MouseButtonFlags flags = SDL_GetMouseState(&mouse->x, &mouse->y);

	mouse->left_button.pressed = (flags & SDL_BUTTON_LMASK) ? 1 : 0;
	mouse->right_button.pressed = (flags & SDL_BUTTON_RMASK) ? 1 : 0;
	mouse->middle_button.pressed = (flags & SDL_BUTTON_MMASK) ? 1 : 0;
}

void *platform_gl_proc(const char *name) {
	void *proc = (void *)SDL_GL_GetProcAddress(name);
	LOG_INFO("GL proc: %s => %p", name, proc);
	return proc;
}
