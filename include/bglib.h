#ifndef BGLIB_H_
#define BGLIB_H_

#include "maler.h"

void bg_init(char* title, int width, int height);

void bg_registry_shader(int key, char* src);
MalerElement *bg_create(MalerBox box, void *data, int data_count, int type, int layer_id);

MalerLayer *bg_get_layer(int id);

void bg_run(void);
int bg_should_close(void);
#endif // BGLIB_H_
