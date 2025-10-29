#include "bglib.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "maler.h"
#include "nsdl.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

#ifdef DEBUG
#include "log.h"
#endif

NSDLWindow g_window;
Renderer g_renderer;
TextureManager g_texture_manager;

const char *vertex_shader_src = R"glsl(
#version 430 core
layout(location = 0) in vec2 aVertex;
uniform vec2 uWindowSize;
uniform vec2 uContainerOffset;
uniform uint uInstanceOffset;

flat out uint vInstanceID;
out vec2 vLocalPos;
out vec2 uSize;

struct InstanceData {
	vec4 box;
	uint data_offset;
	uint data_count;
	uint padding0;
	uint padding1;
};

layout(std430, binding = 0) buffer Instances { InstanceData instances[]; };

void main() {
	uint instanceIndex = gl_InstanceID + uInstanceOffset;
	vInstanceID = instanceIndex;

	InstanceData inst = instances[instanceIndex];

	vec2 pos = (uContainerOffset.xy + inst.box.xy) + aVertex * inst.box.zw;
	vec2 ndc = vec2(2.0 * pos.x / uWindowSize.x - 1.0, 1.0 - 2.0 * pos.y / uWindowSize.y);

	gl_Position = vec4(ndc, 0.0, 1.0);
	vLocalPos = aVertex * inst.box.zw - 0.5 * inst.box.zw;
	uSize = inst.box.zw;
}

)glsl";

void bg_init(char *title, int width, int height) {
	g_window = (NSDLWindow){.title = title, .width = width, .height = height};
	nsdl_init(&g_window);
	renderer_init(&g_renderer, 800, 600);

	g_window.renderer = &g_renderer;
	g_window.draw = renderer_flush;

	texture_manager_init(&g_texture_manager);
}

int bg_load_texture(int id, const char *filename) {
	SDL_Surface *surface = IMG_Load(filename);

	if (!surface) {
#ifdef DEBUG
		LOG_ERROR("bg_load_texture(): %s", SDL_GetError());
#endif
		return -1;
	}
	int tid = texture_manager_add(&g_texture_manager, id, surface->w, surface->h,
							   surface->pixels, surface->format);
	SDL_DestroySurface(surface);
	return tid;
}

Texture *bg_get_texture(int id) {
	return texture_manager_get(&g_texture_manager, id);
}

void bg_register_shader(int key, char *src) {
	shader_register(g_renderer.shaders, key,
					shader_create(vertex_shader_src, src));
}

MalerElement *bg_create(MalerBox box, void *data, int data_count, int type,
						Texture *texture, int container_id) {
	MalerContainer *container =
		renderer_get_container(&g_renderer, container_id);

	if (!container) {
		container = renderer_add_container(&g_renderer, container_id);
	}

	return maler_create(box, data, data_count, type, texture, container);
}

MalerContainer *bg_get_container(int id) {
	return renderer_get_container(&g_renderer, id);
}

int bg_should_close(void) {
	SDL_Event event;
	int running = 1;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			running = 0;
			return running;
			break;
		}
	}

	nsdl_draw(&g_window);

	return running;
}
