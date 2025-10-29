#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>

#include "maler.h"
#include "shader.h"
#include "texture.h"

void renderer_draw_container(Renderer *r, MalerContainer *container, int type) {
	size_t element_count = maler_container_update(container, type);
	if (element_count == 0) return;

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	GLuint shader = shader_get(r->shaders, type);
	glUseProgram(shader);

	glUniform2f(glGetUniformLocation(shader, "uContainerOffset"),
				container->offset_x, container->offset_y);
	glUniform2f(glGetUniformLocation(shader, "uWindowSize"),
				(float)r->win_width, (float)r->win_height);

	glBindVertexArray(r->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->quad_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, container->instance_SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, container->data_SSBO);

	size_t start = 0;
	while (start < element_count) {
		MalerElement *first = container->elements[start];
		Texture *texture = first->texture;

		size_t end = start + 1;
		while (end < element_count &&
			   container->elements[end]->texture == texture) {
			end++;
		}

		if (texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->data);
			glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);
			glUniform1i(glGetUniformLocation(shader, "uUseTexture"), GL_TRUE);
		} else {
			glUniform1i(glGetUniformLocation(shader, "uUseTexture"), GL_FALSE);
		}

		glUniform1ui(glGetUniformLocation(shader, "uInstanceOffset"), start);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, end - start);

		start = end;
	}
}

MalerContainer *renderer_add_container(Renderer *r, int id) {
	r->containers = realloc(r->containers, (r->container_count + 1) *
											   sizeof(MalerContainer *));
	r->containers[r->container_count] = malloc(sizeof(MalerContainer));
	MalerContainer *container = r->containers[r->container_count];
	maler_container_init(container, id);
	r->container_count++;

	return container;
}

MalerContainer *renderer_get_container(Renderer *r, int id) {
	for (int i = 0; i < r->container_count; ++i) {
		if (r->containers[i]->id == id) return r->containers[i];
	}

	return 0;
}

void renderer_init(Renderer *r, int w, int h) {
	r->containers = NULL;
	r->container_count = 0;
	r->shaders = shader_create_registry(64);

	glGenVertexArrays(1, &r->vao);
	r->win_width = w;
	r->win_height = h;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);
}

static int compare_containers(const void *a, const void *b) {
	MalerContainer *la = *(MalerContainer **)a;
	MalerContainer *lb = *(MalerContainer **)b;
	return la->id - lb->id;
}

static int compare_ints(const void *a, const void *b) {
	int ia = *(int *)a;
	int ib = *(int *)b;
	return ia - ib;
}

void renderer_flush(Renderer *r) {
	if (r->container_count == 0) return;

	qsort(r->containers, r->container_count, sizeof(MalerContainer *),
		  compare_containers);

	for (int i = 0; i < r->container_count; ++i) {
		MalerContainer *container = r->containers[i];
		size_t key_count;
		int *keys = shader_registry_get_keys(r->shaders, &key_count);

		qsort(keys, key_count, sizeof(int), compare_ints);

		for (size_t j = 0; j < key_count; ++j) {
			int type = keys[j];

			int has_type = 0;
			for (size_t k = 0; k < container->element_count; k++) {
				if (container->elements[k]->type == type) {
					has_type = 1;
					break;
				}
			}
			if (!has_type) continue;

			renderer_draw_container(r, container, type);
		}

		free(keys);
	}
}
