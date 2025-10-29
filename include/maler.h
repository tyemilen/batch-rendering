#ifndef MALER_H_
#define MALER_H_

#include <GL/glew.h>
#include <limits.h>
#include <stddef.h>

#include "texture.h"

typedef struct MalerBox {
	int x, y, width, height;
} MalerBox;

typedef struct {
	int type;
	void *data;
	int data_count;

	Texture *texture;

	MalerBox box;
	struct MalerContainer *container;
} MalerElement;

typedef struct MalerContainer {
	int id;

	MalerElement **elements;
	size_t element_count;

	GLuint quad_VBO;

	GLuint instance_SSBO;
	GLuint data_SSBO;

	float offset_x, offset_y;
} MalerContainer;

typedef struct {
	float box[4];
	unsigned int data_offset;
	unsigned int data_count;
	unsigned int padding[2]; // std430 thing ig
} InstanceData;

void maler_container_init(MalerContainer *container, int id);
void maler_container_ensure_capacity(MalerContainer *container);
size_t maler_container_update(MalerContainer *container, int type);

MalerElement *maler_create(MalerBox box, void *data, int data_count, int type, Texture *texture, MalerContainer *container);

#endif // MALER_H_
