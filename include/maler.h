#ifndef MALER_H_
#define MALER_H_

#include <limits.h>
#include <stddef.h>

#include "core.h"
#include "texture.h"

typedef struct MalerBox {
	int x, y, width, height;
} MalerBox;

typedef struct {
	int type;

	void *instance;
	size_t instance_size;

	Texture *texture;
	struct MalerContainer *container;
} MalerElement;

typedef struct MalerContainer {
	int id;
	int shader_type;
	int texture_id;
	int shader_bound;

	GLuint shader;

	MalerElement **elements;
	size_t element_count;

	GLuint quad_VBO;
	GLuint instance_VBO;
	GLuint vao;
} MalerContainer;

void maler_container_init(MalerContainer *container, int id, int shader_type);
void maler_container_ensure_capacity(MalerContainer *container);
size_t maler_container_update(MalerContainer *container);

MalerElement *maler_create(void *data, int data_count, int type,
						   Texture *texture, MalerContainer *container);

#endif // MALER_H_
