#ifndef OBJECT_H_
#define OBJECT_H_

#include "maler.h"

typedef void (*obj_update_fn)(void *self);

typedef struct ObjectBase {
	int visible;
	void *instance;
	MalerElement *element;

	obj_update_fn update;
} ObjectBase;

#define MAP_COLOR(v, color)                                                    \
	v[0] = color.r / 255.0f;                                                   \
	v[1] = color.g / 255.0f;                                                   \
	v[2] = color.b / 255.0f;                                                   \
	v[3] = color.a;

#endif // OBJECT_H_
