#include "renderer.h"

#include <stddef.h>
#include <stdlib.h>

#include "log.h"
#include "maler.h"
#include "shader.h"
#include "texture.h"
#include "ygl.h"

void renderer_draw_container(Renderer *r, MalerContainer *container) {
	size_t element_count = maler_container_update(container);
	if (element_count == 0) return;

	if (!container->uContainerOffsetLoc) {
		container->uContainerOffsetLoc =
			glGetUniformLocation(container->shader, "uContainerOffset");
		container->uWindowSizeLoc =
			glGetUniformLocation(container->shader, "uWindowSize");
		container->uTextureLoc =
			glGetUniformLocation(container->shader, "uTexture");
		container->uUseTextureLoc =
			glGetUniformLocation(container->shader, "uUseTexture");
	}

	glUseProgram(container->shader);
	glUniform2f(container->uWindowSizeLoc, r->win_width, r->win_height);
	glBindVertexArray(container->vao);

	size_t start = 0;
	while (start < element_count) {
		MalerElement *first = container->elements[start];
		Texture *texture = first->texture;

		size_t end = start + 1;
		while (end < element_count &&
			   container->elements[end]->texture == texture)
			end++;

		if (texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->data);
			glUniform1i(container->uTextureLoc, 0);
			glUniform1i(container->uUseTextureLoc, GL_TRUE);
		} else {
			glUniform1i(container->uUseTextureLoc, GL_FALSE);
		}
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, end - start);
		start = end;
	}
}

MalerContainer *renderer_add_container(Renderer *r, int shader_type,
									   int texture_id) {
	r->containers = realloc(r->containers, (r->container_count + 1) *
											   sizeof(MalerContainer *));
	r->containers[r->container_count] = malloc(sizeof(MalerContainer));

	MalerContainer *container = r->containers[r->container_count];
	maler_container_init(container, r->container_count, shader_type);

	container->texture_id = texture_id;

	ShaderEntry shader = shader_get(r->shaders, shader_type);
	container->shader = shader.prog;

	r->container_count++;

	shader.bind(container);

	LOG_INFO("CONTAINER + 1");
	return container;
}

MalerContainer *renderer_get_container_by_texture(Renderer *r, int id) {
	for (int i = 0; i < r->container_count; ++i) {
		if (r->containers[i]->texture_id == id) return r->containers[i];
	}

	return 0;
}

MalerContainer *renderer_get_container_by_shader(Renderer *r, int type) {
	for (int i = 0; i < r->container_count; ++i) {
		if (r->containers[i]->shader_type == type &&
			!r->containers[i]->texture_id)
			return r->containers[i];
	}

	return 0;
}

void renderer_init(Renderer *r, int w, int h) {
	r->containers = NULL;
	r->container_count = 0;
	r->shaders = shader_create_registry(64);

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

void renderer_flush(Renderer *r) {
	if (r->container_count == 0) return;

	qsort(r->containers, r->container_count, sizeof(MalerContainer *),
		  compare_containers);

	for (int i = 0; i < r->container_count; ++i) {
		MalerContainer *container = r->containers[i];

		renderer_draw_container(r, container);
	}
}
