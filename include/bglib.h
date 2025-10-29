#ifndef BGLIB_H_
#define BGLIB_H_

#include "maler.h"
#include "texture.h"

void bg_init(char* title, int width, int height);

void bg_register_shader(int key, char* src);
MalerElement *bg_create(MalerBox box, void *data, int data_count, int type, Texture *texture, int container_id);

int bg_load_texture(int id, const char* filename);
Texture *bg_get_texture(int id);

MalerContainer *bg_get_container(int id);

void bg_run(void);
int bg_should_close(void);
#endif // BGLIB_H_
