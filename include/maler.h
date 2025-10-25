#ifndef MALER_H_
#define MALER_H_

#include <GL/glew.h>
#include <limits.h>
#include <stddef.h>

typedef struct MalerBox {
	int x, y, width, height;
} MalerBox;

typedef struct {
	int type;
	void *data;
	int data_count;

	MalerBox box;
	struct MalerLayer *layer;
} MalerElement;

typedef struct MalerLayer {
	int id;

	MalerElement **elements;
	size_t element_count;

	GLuint quad_VBO;

	GLuint instance_SSBO;
	GLuint data_SSBO;

	float offset_x, offset_y;
} MalerLayer;

typedef struct {
	float box[4];
	unsigned int data_offset;
	unsigned int data_count;
	unsigned int padding[2]; // std430 thing ig
} InstanceData;

void maler_layer_init(MalerLayer *layer, int id);
void maler_layer_ensure_capacity(MalerLayer *layer);
int maler_layer_update(MalerLayer *layer, int type);

MalerElement *maler_create(MalerBox box, void *data, int data_count, int type,
						   MalerLayer *layer);

#endif // MALER_H_
