#ifndef YTA_H_
#define YTA_H_

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "texture.h"

void yta_init(char *title, int width, int height);

void yta_register_shader(int key, GLuint prog, void (*bind)(MalerContainer *));

MalerElement *yta_create(void *instance, int instance_size, int type,
						 Texture *texture);
MalerElement *yta_create_ex(void *instance, int instance_size, int type,
							Texture *texture, MalerContainer *container);
MalerContainer *yta_create_container(int shader_type, int texture_id);

void yta_destroy_element(MalerContainer *container, size_t index);
void yta_destroy(void);

Mouse yta_get_mouse(void);
Texture *yta_load_texture(int id, const char *filename);
Texture *yta_get_texture(int id);

Window yta_get_window(void);

void yta_run(void);
void yta_clear(Color color);
int yta_should_close(void);

double yta_get_time(void);
float yta_delta(void);

#endif // YTA_H_
