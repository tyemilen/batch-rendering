#include "bglib.h"

#include <SDL3/SDL_events.h>

#include "maler.h"
#include "nsdl.h"
#include "renderer.h"
#include "shader.h"

NSDLWindow g_window;
Renderer g_renderer;
const char *vertex_shader_src = R"glsl(
#version 430 core
layout(location = 0) in vec2 aVertex;
uniform vec2 uWindowSize;
uniform vec2 uLayerOffset;

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
	vInstanceID = gl_InstanceID;
	InstanceData inst = instances[vInstanceID];

	vec2 pos = (uLayerOffset.xy + inst.box.xy) + aVertex * inst.box.zw;
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
}

void bg_registry_shader(int key, char *src) {
	shader_register(g_renderer.shaders, key,
					shader_create(vertex_shader_src, src));
}

MalerElement *bg_create(MalerBox box, void *data, int data_count, int type,
						int layer_id) {
	MalerLayer *layer = renderer_get_layer(&g_renderer, layer_id);

	if (!layer) {
		layer = renderer_add_layer(&g_renderer, layer_id);
	}

	return maler_create(box, data, data_count, type, layer);
}

MalerLayer *bg_get_layer(int id) {
	return renderer_get_layer(&g_renderer, id);
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
