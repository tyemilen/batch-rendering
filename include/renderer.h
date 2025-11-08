#ifndef RENDERER_H_
#define RENDERER_H_

#include "maler.h"
#include "shader.h"

typedef struct Renderer {
	MalerContainer **containers;
	int container_count;

	ShaderRegistry *shaders;

	int win_width, win_height;
} Renderer;

void renderer_draw_container(Renderer *r, MalerContainer *container);

MalerContainer *renderer_add_container(Renderer *r, int shader_type,
									   int texture_id);
MalerContainer *renderer_get_container_by_texture(Renderer *r, int texture_id);
MalerContainer *renderer_get_container_by_shader(Renderer *r, int type);

void renderer_init(Renderer *r, int w, int h);
void renderer_flush(Renderer *r);

#endif // RENDERER_H_
