#ifndef NSDL_H_
#define NSDL_H_

#include <SDL3/SDL_video.h>

#include "renderer.h"

typedef struct SDL_Window SDL_Window;

typedef struct NSDLWindow {
	char* title;

	int running;
	int state;
	int width, height;

	SDL_Window *window;
	SDL_GLContext gl_context;

	struct Renderer *renderer;

	void (*draw)(struct Renderer *);
	void (*update)(struct NSDLWindow *);
} NSDLWindow;

int nsdl_init(NSDLWindow* window);
int nsdl_run(NSDLWindow* window);

void nsdl_pollevents(NSDLWindow *window);

void nsdl_update(NSDLWindow *window);
void nsdl_draw(NSDLWindow *window);

#endif // NSDL_H_
