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

#endif // OBJECT_H_
