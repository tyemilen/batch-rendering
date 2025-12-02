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
#include "log.h"
#include "ygl.h"
#include "yta.h"

typedef struct {
	SDL_Window *window;
	SDL_GLContext glContext;

	SDL_Cursor *cursor;
	int keys_state[MAX_KEYBOARD_KEYS];
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

KeyboardKey sc_to_yta(int sc) {
	switch (sc) {
	case SDLK_SPACE:
		return KEY_SPACE;
	case SDLK_APOSTROPHE:
		return KEY_APOSTROPHE;
	case SDLK_COMMA:
		return KEY_COMMA;
	case SDLK_MINUS:
		return KEY_MINUS;
	case SDLK_PERIOD:
		return KEY_PERIOD;
	case SDLK_SLASH:
		return KEY_SLASH;
	case SDLK_0:
		return KEY_0;
	case SDLK_1:
		return KEY_1;
	case SDLK_2:
		return KEY_2;
	case SDLK_3:
		return KEY_3;
	case SDLK_4:
		return KEY_4;
	case SDLK_5:
		return KEY_5;
	case SDLK_6:
		return KEY_6;
	case SDLK_7:
		return KEY_7;
	case SDLK_8:
		return KEY_8;
	case SDLK_9:
		return KEY_9;
	case SDLK_SEMICOLON:
		return KEY_SEMICOLON;
	case SDLK_EQUALS:
		return KEY_EQUAL;
	case SDLK_A:
		return KEY_A;
	case SDLK_B:
		return KEY_B;
	case SDLK_C:
		return KEY_C;
	case SDLK_D:
		return KEY_D;
	case SDLK_E:
		return KEY_E;
	case SDLK_F:
		return KEY_F;
	case SDLK_G:
		return KEY_G;
	case SDLK_H:
		return KEY_H;
	case SDLK_I:
		return KEY_I;
	case SDLK_J:
		return KEY_J;
	case SDLK_K:
		return KEY_K;
	case SDLK_L:
		return KEY_L;
	case SDLK_M:
		return KEY_M;
	case SDLK_N:
		return KEY_N;
	case SDLK_O:
		return KEY_O;
	case SDLK_P:
		return KEY_P;
	case SDLK_Q:
		return KEY_Q;
	case SDLK_R:
		return KEY_R;
	case SDLK_S:
		return KEY_S;
	case SDLK_T:
		return KEY_T;
	case SDLK_U:
		return KEY_U;
	case SDLK_V:
		return KEY_V;
	case SDLK_W:
		return KEY_W;
	case SDLK_X:
		return KEY_X;
	case SDLK_Y:
		return KEY_Y;
	case SDLK_Z:
		return KEY_Z;
	case SDLK_LEFTBRACKET:
		return KEY_LEFT_BRACKET;
	case SDLK_BACKSLASH:
		return KEY_BACKSLASH;
	case SDLK_RIGHTBRACKET:
		return KEY_RIGHT_BRACKET;
	case SDLK_GRAVE:
		return KEY_GRAVE;
	case SDLK_ESCAPE:
		return KEY_ESCAPE;
	case SDLK_RETURN:
		return KEY_ENTER;
	case SDLK_TAB:
		return KEY_TAB;
	case SDLK_BACKSPACE:
		return KEY_BACKSPACE;
	case SDLK_INSERT:
		return KEY_INSERT;
	case SDLK_DELETE:
		return KEY_DELETE;
	case SDLK_RIGHT:
		return KEY_RIGHT;
	case SDLK_LEFT:
		return KEY_LEFT;
	case SDLK_DOWN:
		return KEY_DOWN;
	case SDLK_UP:
		return KEY_UP;
	case SDLK_PAGEUP:
		return KEY_PAGE_UP;
	case SDLK_PAGEDOWN:
		return KEY_PAGE_DOWN;
	case SDLK_HOME:
		return KEY_HOME;
	case SDLK_END:
		return KEY_END;
	case SDLK_CAPSLOCK:
		return KEY_CAPS_LOCK;
	case SDLK_SCROLLLOCK:
		return KEY_SCROLL_LOCK;
	case SDLK_NUMLOCKCLEAR:
		return KEY_NUM_LOCK;
	case SDLK_PRINTSCREEN:
		return KEY_PRINT_SCREEN;
	case SDLK_PAUSE:
		return KEY_PAUSE;
	case SDLK_F1:
		return KEY_F1;
	case SDLK_F2:
		return KEY_F2;
	case SDLK_F3:
		return KEY_F3;
	case SDLK_F4:
		return KEY_F4;
	case SDLK_F5:
		return KEY_F5;
	case SDLK_F6:
		return KEY_F6;
	case SDLK_F7:
		return KEY_F7;
	case SDLK_F8:
		return KEY_F8;
	case SDLK_F9:
		return KEY_F9;
	case SDLK_F10:
		return KEY_F10;
	case SDLK_F11:
		return KEY_F11;
	case SDLK_F12:
		return KEY_F12;
	case SDLK_LSHIFT:
		return KEY_LEFT_SHIFT;
	case SDLK_LCTRL:
		return KEY_LEFT_CONTROL;
	case SDLK_LALT:
		return KEY_LEFT_ALT;
	case SDLK_LGUI:
		return KEY_LEFT_SUPER;
	case SDLK_RSHIFT:
		return KEY_RIGHT_SHIFT;
	case SDLK_RCTRL:
		return KEY_RIGHT_CONTROL;
	case SDLK_RALT:
		return KEY_RIGHT_ALT;
	case SDLK_RGUI:
		return KEY_RIGHT_SUPER;
	case SDLK_MENU:
		return KEY_MENU;
	case SDLK_KP_0:
		return KEY_KP_0;
	case SDLK_KP_1:
		return KEY_KP_1;
	case SDLK_KP_2:
		return KEY_KP_2;
	case SDLK_KP_3:
		return KEY_KP_3;
	case SDLK_KP_4:
		return KEY_KP_4;
	case SDLK_KP_5:
		return KEY_KP_5;
	case SDLK_KP_6:
		return KEY_KP_6;
	case SDLK_KP_7:
		return KEY_KP_7;
	case SDLK_KP_8:
		return KEY_KP_8;
	case SDLK_KP_9:
		return KEY_KP_9;
	case SDLK_KP_DECIMAL:
		return KEY_KP_DECIMAL;
	case SDLK_KP_DIVIDE:
		return KEY_KP_DIVIDE;
	case SDLK_KP_MULTIPLY:
		return KEY_KP_MULTIPLY;
	case SDLK_KP_MINUS:
		return KEY_KP_SUBTRACT;
	case SDLK_KP_PLUS:
		return KEY_KP_ADD;
	case SDLK_KP_ENTER:
		return KEY_KP_ENTER;
	case SDLK_KP_EQUALS:
		return KEY_KP_EQUAL;
	}

	return KEY_NULL;
}
void platform_draw(void) { SDL_GL_SwapWindow(platform.window); }

int platform_get_key(int k) { return platform.keys_state[k]; }

void handle_key(int kkey, int state) {
	KeyboardKey key = sc_to_yta(kkey);

	platform.keys_state[key] = state;
}

void platform_poll_event(Event *event) {
	SDL_Event sdl_event;
	SDL_PollEvent(&sdl_event);

	event->data1 = 0;
	switch (sdl_event.type) {
	case SDL_EVENT_QUIT:
		event->type = EVENT_EXIT;
		break;
	case SDL_EVENT_KEY_DOWN:
		handle_key(sdl_event.key.key, 1);
		break;
	case SDL_EVENT_KEY_UP:
		handle_key(sdl_event.key.key, 0);
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
