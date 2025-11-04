#include "bglib.h"

#include "maler.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "core.h"
#include <stdlib.h>

#ifdef DEBUG
#include "log.h"
#endif

Window window = {0};
Renderer g_renderer;
TextureManager g_texture_manager;

void bg_init(char *title, int width, int height) {
	window = core_create_window(title, width, height);
	core_init_gl(window);

	renderer_init(&g_renderer, 800, 600);
	texture_manager_init(&g_texture_manager);
}

int bg_load_texture(int id, const char *filename) {
	if (id < 0) return -1;

	Image *image = core_load_image(filename);

	int tid = texture_manager_add(&g_texture_manager, id, image);

	free(image);
	return tid;
}

Texture *bg_get_texture(int id) {
	return texture_manager_get(&g_texture_manager, id);
}

void bg_register_shader(int key, const char *vertex, const char *frag) {
	shader_register(g_renderer.shaders, key, shader_create(vertex, frag));
}

MalerElement *bg_create(void *data, int data_count, int type,
						Texture *texture) {
	MalerContainer *container =
		texture ? renderer_get_container_by_texture(&g_renderer, texture->id)
				: renderer_get_container_by_shader(&g_renderer, type);

	if (!container) {
		container = renderer_add_container(&g_renderer, type, type,
										   texture ? texture->id : 0);
	}

	return maler_create(data, data_count, type, texture, container);
}

MalerContainer *bg_get_container_by_shader(int shader_type) {
	return renderer_get_container_by_shader(&g_renderer, shader_type);
}

MalerContainer *bg_get_container_by_texture(int texture_id) {
	return renderer_get_container_by_texture(&g_renderer, texture_id);
}

int bg_should_close(void) {
	renderer_flush(&g_renderer);
	Event event;
	int running = 1;

	while (core_poll_event(&event)) {
		switch (event.type) {
		case EVENT_EXIT:
			running = 0;
			return running;
			break;
		}
	}

	core_draw();

	return running;
}
