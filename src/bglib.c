#include "bglib.h"

#include <pthread_time.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "maler.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

Window window = {0};
Renderer g_renderer;
TextureManager g_texture_manager;

void bg_init(char *title, int width, int height) {
	window = core_create_window(title, width, height);
	core_init_gl(window);
	renderer_init(&g_renderer, 800, 600);
	texture_manager_init(&g_texture_manager);
}

Mouse bg_get_mouse(void) {
	core_get_mouse(&window.mouse);
	return window.mouse;
}

Texture *bg_load_texture(int id, const char *filename) {
	if (id < 0) return NULL;

	Image *image = core_load_image(filename);

	Texture *texture = texture_manager_add(&g_texture_manager, id, image);

	free(image);
	return texture;
}

Texture *bg_get_texture(int id) {
	return texture_manager_get(&g_texture_manager, id);
}

void bg_register_shader(int key, GLuint prog, void (*bind)(MalerContainer *)) {
	shader_register(g_renderer.shaders, key, prog, bind);
}

MalerElement *bg_create(void *instance, int instance_size, int type,
						Texture *texture) {
	MalerContainer *container =
		texture ? renderer_get_container_by_texture(&g_renderer, texture->id)
				: renderer_get_container_by_shader(&g_renderer, type);

	if (!container) {
		container = renderer_add_container(&g_renderer, type,
										   texture ? texture->id : 0);
	}

	return maler_create(instance, instance_size, type, texture, container);
}

MalerElement *bg_create_ex(void *instance, int instance_size, int type,
						   Texture *texture, MalerContainer *container) {
	return maler_create(instance, instance_size, type, texture, container);
}

MalerContainer *bg_create_container(int shader_type, int texture_id) {
	return renderer_add_container(&g_renderer, shader_type, texture_id);
}

MalerContainer *bg_get_container_by_shader(int shader_type) {
	return renderer_get_container_by_shader(&g_renderer, shader_type);
}

MalerContainer *bg_get_container_by_texture(int texture_id) {
	return renderer_get_container_by_texture(&g_renderer, texture_id);
}

Window bg_get_window(void) { return window; }

int bg_should_close(void) {
	renderer_flush(&g_renderer);
	Event event;
	int exit = 0;

	while (core_poll_event(&event)) {
		switch (event.type) {
		case EVENT_EXIT:
			exit = 1;
			return exit;
		}
	}

	core_draw();

	return exit;
}

double bg_get_time(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

float bg_delta(void) {
	static double prev_time = 0.0;
	double cur_time = bg_get_time();
	float delta = (float)(cur_time - prev_time);
	prev_time = cur_time;
	return delta;
}
