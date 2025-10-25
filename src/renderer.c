#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>

#include "maler.h"
#include "shader.h"

void renderer_draw_layer(Renderer *r, MalerLayer *layer, int type) {
	int element_count =
		maler_layer_update(layer, type);
	if (element_count == 0) return;
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	GLuint shader = shader_get(r->shaders, type);

	glUseProgram(shader);

	glUniform2f(glGetUniformLocation(shader, "uLayerOffset"), layer->offset_x,
				layer->offset_y);
	glUniform2f(glGetUniformLocation(shader, "uWindowSize"),
				(float)r->win_width, (float)r->win_height);

	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, layer->quad_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, layer->instance_SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, layer->data_SSBO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, element_count);
}

MalerLayer *renderer_add_layer(Renderer *r, int id) {
	r->layers = realloc(r->layers, (r->layer_count + 1) * sizeof(MalerLayer *));
	r->layers[r->layer_count] = malloc(sizeof(MalerLayer));
	MalerLayer *layer = r->layers[r->layer_count];
	maler_layer_init(layer, id);
	r->layer_count++;

	return layer;
}

MalerLayer *renderer_get_layer(Renderer *r, int id) {
	for (int i = 0; i < r->layer_count; ++i) {
		if (r->layers[i]->id == id) return r->layers[i];
	}

	return 0;
}

void renderer_init(Renderer *r, int w, int h) {
	r->layers = NULL;
	r->layer_count = 0;
	r->shaders = shader_create_registry(64);

	glGenVertexArrays(1, &r->vao);
	r->win_width = w;
	r->win_height = h;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);
}

static int compare_layers(const void *a, const void *b) {
	MalerLayer *la = *(MalerLayer **)a;
	MalerLayer *lb = *(MalerLayer **)b;
	return la->id - lb->id;
}

static int compare_ints(const void *a, const void *b) {
	int ia = *(int *)a;
	int ib = *(int *)b;
	return ia - ib;
}

void renderer_flush(Renderer *r) {
	if (r->layer_count == 0) return;

	qsort(r->layers, r->layer_count, sizeof(MalerLayer *), compare_layers);

	for (int i = 0; i < r->layer_count; ++i) {
		MalerLayer *layer = r->layers[i];
		size_t key_count;
		int *keys = shader_registry_get_keys(r->shaders, &key_count);

		qsort(keys, key_count, sizeof(int), compare_ints);

		for (size_t j = 0; j < key_count; ++j) {
			int type = keys[j];

			int has_type = 0;
			for (size_t k = 0; k < layer->element_count; k++) {
				if (layer->elements[k]->type == type) {
					has_type = 1;
					break;
				}
			}
			if (!has_type) continue;

			renderer_draw_layer(r, layer, type);
		}

		free(keys);
	}
}
