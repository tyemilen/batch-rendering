#include "yta.h"

#include <bits/time.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "graphics/grid.h"
#include "maler.h"
#include "objects/object.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "ygl.h"

#include "graphics/rect.h"
#include "graphics/text.h"

Window g_window = {0};
Renderer g_renderer;
TextureManager g_texture_manager;

static ObjectBase **g_objects = NULL;
static size_t g_obj_count = 0;

void YtaRegisterObject(ObjectBase *obj) {
	g_objects = realloc(g_objects, sizeof(*g_objects) * (g_obj_count + 1));
	g_objects[g_obj_count] = obj;
	++g_obj_count;
}

void YtaUpdateObjects(void) {
	for (size_t i = 0; i < g_obj_count; ++i) {
		g_objects[i]->update(g_objects[i]);
	}
}

Window YtaInit(char *title, int width, int height) {
	g_window = core_create_window(title, width, height);
	core_init_gl(g_window);
	renderer_init(&g_renderer, width, height);
	texture_manager_init(&g_texture_manager);

	YtaRegisterShader(SHADER_RECT, shader_rect_get(), shader_rect_bind);
	YtaRegisterShader(SHADER_TEXT, shader_text_get(), shader_text_bind);
	YtaRegisterShader(SHADER_GRID, shader_grid_get(), shader_grid_bind);

	return g_window;
}

Mouse YtaGetMouse(void) {
	core_get_mouse(&g_window.mouse);
	return g_window.mouse;
}

Texture *YtaLoadTexture(int id, const char *filename) {
	if (id < 0) return NULL;

	Image *image = core_load_image(filename);
	Texture *texture = texture_manager_add(&g_texture_manager, id, image);
	free(image);
	return texture;
}

Texture *YtaGetTexture(int id) {
	return texture_manager_get(&g_texture_manager, id);
}

void YtaRegisterShader(int key, GLuint prog, void (*bind)(MalerContainer *)) {
	shader_register(g_renderer.shaders, key, prog, bind);
}

MalerElement *YtaCreate(void *instance, int instance_size, int type,
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

MalerElement *YtaCreateEx(void *instance, int instance_size, int type,
						  Texture *texture, MalerContainer *container) {
	return maler_create(instance, instance_size, type, texture, container);
}

MalerContainer *YtaCreateContainer(int shader_type, int texture_id) {
	return renderer_add_container(&g_renderer, shader_type, texture_id);
}

MalerContainer *YtaGetContainerByShader(int shader_type) {
	return renderer_get_container_by_shader(&g_renderer, shader_type);
}

MalerContainer *YtaGetContainerByTexture(int texture_id) {
	return renderer_get_container_by_texture(&g_renderer, texture_id);
}

Window YtaGetWindow(void) { return g_window; }

void YtaClear(Color color) {
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

int YtaShouldClose(void) {
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
	YtaUpdateObjects();
	core_draw();

	return exit;
}

void YtaDestroyElement(MalerContainer *container, size_t index) {
	if (index >= container->element_count) return;
	if (container->elements[index]->instance) {
		free(container->elements[index]->instance);
		container->elements[index]->instance = NULL;
	}
	free(container->elements[index]);
}

void YtaDestroy(void) {
	for (int i = 0; i < g_renderer.container_count; ++i) {
		for (size_t j = 0; j < g_renderer.containers[i]->element_count; ++j) {
			YtaDestroyElement(g_renderer.containers[i], j);
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

double YtaGetTime(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

float YtaDelta(void) {
	static double prev_time = 0.0;
	double cur_time = YtaGetTime();
	float delta = (float)(cur_time - prev_time);
	prev_time = cur_time;
	return delta;
}