#include "maler.h"

#include <GL/glew.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef DEBUG
#include "log.h"
#endif
#include "texture.h"

const float quad_vertices[12] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

void maler_container_init(MalerContainer *container, int id) {
	container->id = id;
	container->elements = 0;
	container->element_count = 0;
	container->offset_x = 0;
	container->offset_y = 0;

	glGenBuffers(1, &container->quad_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, container->quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices,
				 GL_STATIC_DRAW);

	glGenBuffers(1, &container->instance_SSBO);
	glGenBuffers(1, &container->data_SSBO);
}

size_t maler_container_update(MalerContainer *container, int type) {
	size_t type_count = 0;
	size_t total_floats = 0;
	for (size_t i = 0; i < container->element_count; i++) {
		if (container->elements[i]->type != type) continue;
		type_count++;
		total_floats += container->elements[i]->data_count;
	}

	if (type_count == 0) return 0;

	float *data_buffer = malloc(total_floats * sizeof(float));
	InstanceData *instances = malloc(type_count * sizeof(InstanceData));

	size_t offset = 0;
	size_t idx = 0;
	for (size_t i = 0; i < container->element_count; i++) {
		if (container->elements[i]->type != type) continue;

		MalerElement *el = container->elements[i];

		instances[idx].box[0] = el->box.x;
		instances[idx].box[1] = el->box.y;
		instances[idx].box[2] = el->box.width;
		instances[idx].box[3] = el->box.height;
		instances[idx].data_offset = offset;
		instances[idx].data_count = el->data_count;
		instances[idx].padding[0] = 0;
		instances[idx].padding[1] = 0;

		for (int j = 0; j < el->data_count; j++)
			data_buffer[offset++] = ((float *)el->data)[j];

		idx++;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, container->instance_SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, type_count * sizeof(InstanceData),
				 instances, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, container->data_SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, total_floats * sizeof(float),
				 data_buffer, GL_DYNAMIC_DRAW);

	free(instances);
	free(data_buffer);

	return type_count;
}

MalerElement *maler_create(MalerBox box, void *data, int data_count, int type, Texture *texture, MalerContainer *container) {
#ifdef DEBUG
	LOG_INFO("maler_create(): +1");
#endif
	container->elements = realloc(container->elements, (container->element_count + 1) *
												   sizeof(MalerElement *));
	MalerElement *element = malloc(sizeof(MalerElement));

	element->data = data;
	element->data_count = data_count;
	element->type = type;
	element->box = box;
	element->texture = texture;
	element->container = container;

	container->elements[container->element_count] = element;
	container->element_count += 1;

	return element;
}
