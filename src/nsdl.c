#include "nsdl.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <gl/glew.h>

#ifdef DEBUG
#include "log.h"
#endif

int nsdl_init(NSDLWindow *window) {
	if (!SDL_Init(SDL_INIT_VIDEO)) return 1;

	SDL_Window *swindow =
		SDL_CreateWindow(window->title, window->width, window->height,
						 SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GLContext ctx = SDL_GL_CreateContext(swindow);
	SDL_GL_MakeCurrent(swindow, ctx);
	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	int gerr = GLEW_OK;
	if ((gerr = glewInit()) != GLEW_OK) {
#ifdef DEBUG
		LOG_ERROR("nsdl_init(): %s", glewGetErrorString(gerr));
#endif
		return 0;
	};
	glViewport(0, 0, 800, 600);

	window->window = swindow;
	window->gl_context = ctx;
	window->running = 0;

	return 1;
}

int nsdl_run(NSDLWindow *window) {
	window->running = 1;

	while (window->running) {
		nsdl_pollevents(window);
		nsdl_update(window);
		nsdl_draw(window);
	}

	SDL_GL_DestroyContext(window->gl_context);
	SDL_DestroyWindow(window->window);

	SDL_Quit();

	return 0;
}

void nsdl_pollevents(NSDLWindow *window) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			window->running = 0;
			break;
		}
	}
}

void nsdl_update(NSDLWindow *window) { window->update(window); }

void nsdl_draw(NSDLWindow *window) {
	int start = SDL_GetTicks();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	window->draw(window->renderer);
	SDL_GL_SwapWindow(window->window);

	int end = SDL_GetTicks();
	if (end - start < 16) SDL_Delay(16 - (end - start));
}
