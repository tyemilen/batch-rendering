#ifndef YTA_H_
#define YTA_H_

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "texture.h"

#define COLOR_RED (Color){255, 0, 0, 1}
#define COLOR_GREEN (Color){0, 255, 0, 1}
#define COLOR_BLUE (Color){0, 0, 255, 1}
#define COLOR_YELLOW (Color){255, 255, 0, 1}
#define COLOR_CYAN (Color){0, 255, 255, 1}
#define COLOR_MAGENTA (Color){255, 0, 255, 1}
#define COLOR_ORANGE (Color){255, 165, 0, 1}
#define COLOR_PURPLE (Color){128, 0, 128, 1}
#define COLOR_PINK (Color){255, 192, 203, 1}
#define COLOR_BROWN (Color){165, 42, 42, 1}
#define COLOR_GRAY (Color){128, 128, 128, 1}
#define COLOR_BLACK (Color){0, 0, 0, 1}
#define COLOR_WHITE (Color){255, 255, 255, 1}

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
