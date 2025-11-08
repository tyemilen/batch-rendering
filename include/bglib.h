#ifndef BGLIB_H_
#define BGLIB_H_

#include "core.h"
#include "maler.h"
#include "texture.h"

void bg_init(char *title, int width, int height);

void bg_register_shader(int key, GLuint prog, void (*bind)(MalerContainer *));

MalerElement *bg_create(void *instance, int instance_size, int type,
						Texture *texture);
MalerElement *bg_create_ex(void *instance, int instance_size, int type,
						   Texture *texture, MalerContainer *container);
MalerContainer *bg_create_container(int shader_type, int texture_id);

Mouse bg_get_mouse(void);
Texture *bg_load_texture(int id, const char *filename);
Texture *bg_get_texture(int id);

Window bg_get_window(void);

void bg_run(void);
int bg_should_close(void);

double bg_get_time(void);
float bg_delta(void);
#endif // BGLIB_H_
