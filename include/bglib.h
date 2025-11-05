#ifndef BGLIB_H_
#define BGLIB_H_

#include "core.h"
#include "maler.h"
#include "texture.h"

void bg_init(char* title, int width, int height);

void bg_register_shader(int key, GLuint prog, void (*bind)(MalerContainer *));

MalerElement *bg_create(void *data, int data_count, int type, Texture *texture);
Mouse bg_get_mouse(void);
Texture *bg_load_texture(int id, const char *filename);
Texture *bg_get_texture(int id);

void bg_run(void);
int bg_should_close(void);

float bg_delta(void);
#endif // BGLIB_H_
