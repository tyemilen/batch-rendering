#ifndef RENDERER_H_
#define RENDERER_H_

#include "maler.h"
#include "shader.h"

typedef struct Renderer {
	MalerContainer** containers;
	int container_count;

	GLuint vao;
	ShaderRegistry *shaders;

	int win_width, win_height;
} Renderer;

void renderer_draw_container(Renderer *r, MalerContainer *container, int type);
MalerContainer* renderer_add_container(Renderer *r, int id);
MalerContainer* renderer_get_container(Renderer *r, int id);

void renderer_init(Renderer *r, int w, int h);
void renderer_flush(Renderer *r);

#endif // RENDERER_H_
