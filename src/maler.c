#include "maler.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "texture.h"
#include "ygl.h"

const float quad_vertices[12] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

void maler_container_init(MalerContainer *container, int id, int shader_type) {
	container->id = id;
	container->elements = NULL;
	container->element_count = 0;
	container->shader_type = shader_type;
	container->shader_bound = 0;

	container->uContainerOffsetLoc = 0;
	container->uWindowSizeLoc = 0;
	container->uTextureLoc = 0;
	container->uUseTextureLoc = 0;

	glGenVertexArrays(1, &container->vao);
	glGenBuffers(1, &container->quad_VBO);
	glGenBuffers(1, &container->instance_VBO);
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices,
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
						  (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);
	glBindVertexArray(0);
}

size_t maler_container_update(MalerContainer *container) {
	size_t count = 0;
	for (size_t i = 0; i < container->element_count; i++) {
		if (container->elements[i]->type == container->shader_type &&
			container->elements[i]->visible)
			count++;
	}

	if (count == 0) return 0;

	size_t total_size = 0;
	for (size_t i = 0; i < container->element_count; i++) {
		if (container->elements[i]->type == container->shader_type) {
			total_size += container->elements[i]->instance_size;
		}
	}

	char *buffer = malloc(total_size);
	size_t offset = 0;
	for (size_t i = 0; i < container->element_count; i++) {
		MalerElement *el = container->elements[i];
		if (el->type != container->shader_type) continue;
		memcpy(buffer + offset, el->instance, el->instance_size);
		offset += el->instance_size;
	}

	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);
	glBufferData(GL_ARRAY_BUFFER, total_size, buffer, GL_DYNAMIC_DRAW);
	free(buffer);

	return count;
}

MalerElement *maler_create(void *instance, int instance_size, int type,
						   Texture *texture, MalerContainer *container) {

	container->elements =
		realloc(container->elements,
				(container->element_count + 1) * sizeof(MalerElement *));
	MalerElement *element = malloc(sizeof(MalerElement));

	element->instance = instance;
	element->instance_size = instance_size;
	element->type = type;
	element->texture = texture;
	element->container = container;
	element->visible = 1;

	container->elements[container->element_count] = element;
	container->element_count += 1;

	return element;
}
