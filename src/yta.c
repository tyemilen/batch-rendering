#include "yta.h"

#include <pthread_time.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "maler.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "graphics/rect.h"
#include "graphics/text.h"

Window window = {0};
Renderer g_renderer;
TextureManager g_texture_manager;

void yta_init(char *title, int width, int height) {
	window = core_create_window(title, width, height);
	core_init_gl(window);
	renderer_init(&g_renderer, 800, 600);
	texture_manager_init(&g_texture_manager);

	yta_register_shader(SHADER_RECT, shader_rect_get(), shader_rect_bind);
	yta_register_shader(SHADER_TEXT, shader_text_get(), shader_text_bind);
}

Mouse yta_get_mouse(void) {
	core_get_mouse(&window.mouse);
	return window.mouse;
}

Texture *yta_load_texture(int id, const char *filename) {
	if (id < 0) return NULL;

	Image *image = core_load_image(filename);

	Texture *texture = texture_manager_add(&g_texture_manager, id, image);

	free(image);
	return texture;
}

Texture *yta_get_texture(int id) {
	return texture_manager_get(&g_texture_manager, id);
}

void yta_register_shader(int key, GLuint prog, void (*bind)(MalerContainer *)) {
	shader_register(g_renderer.shaders, key, prog, bind);
}

MalerElement *yta_create(void *instance, int instance_size, int type,
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

MalerElement *yta_create_ex(void *instance, int instance_size, int type,
							Texture *texture, MalerContainer *container) {
	return maler_create(instance, instance_size, type, texture, container);
}

MalerContainer *yta_create_container(int shader_type, int texture_id) {
	return renderer_add_container(&g_renderer, shader_type, texture_id);
}

MalerContainer *yta_get_container_by_shader(int shader_type) {
	return renderer_get_container_by_shader(&g_renderer, shader_type);
}

MalerContainer *yta_get_container_by_texture(int texture_id) {
	return renderer_get_container_by_texture(&g_renderer, texture_id);
}

Window yta_get_window(void) { return window; }

void yta_clear(Color color) {
	glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

int yta_should_close(void) {
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

void yta_destroy_element(MalerContainer *container, size_t index) {
	if (index >= container->element_count) return;
	if (container->elements[index]->instance) {
		free(container->elements[index]->instance);
		container->elements[index]->instance = NULL;
	}
	free(container->elements[index]);
}

void yta_destroy(void) {
	for (int i = 0; i < g_renderer.container_count; ++i) {
		for (size_t j = 0; j < g_renderer.containers[i]->element_count; ++j) {
			yta_destroy_element(g_renderer.containers[i], j);
		}
		g_renderer.containers[i]->elements = NULL;
		g_renderer.containers[i]->element_count = 0;

		free(g_renderer.containers[i]);
		g_renderer.containers[i] = NULL;
	}
	free(g_renderer.containers);
	g_renderer.containers = NULL;
	g_renderer.container_count = 0;
}

double yta_get_time(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

float yta_delta(void) {
	static double prev_time = 0.0;
	double cur_time = yta_get_time();
	float delta = (float)(cur_time - prev_time);
	prev_time = cur_time;
	return delta;
}
