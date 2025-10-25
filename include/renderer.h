#ifndef RENDERER_H_
#define RENDERER_H_

#include "maler.h"
#include "shader.h"

typedef struct Renderer {
	MalerLayer** layers;
	int layer_count;

	GLuint vao;
	ShaderRegistry *shaders;

	int win_width, win_height;
} Renderer;

void renderer_draw_layer(Renderer *r, MalerLayer *layer, int type);
MalerLayer* renderer_add_layer(Renderer *r, int id);
MalerLayer* renderer_get_layer(Renderer *r, int id);

void renderer_init(Renderer *r, int w, int h);
void renderer_flush(Renderer *r);

#endif // RENDERER_H_
