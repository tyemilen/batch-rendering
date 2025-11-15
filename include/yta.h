#ifndef YTA_H_
#define YTA_H_

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "texture.h"

#include "objects/object.h"
#include "ygl.h"

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
#define COLOR_NONE (Color){0, 0, 0, 0}
#define COLOR_WHITE (Color){255, 255, 255, 1}

void YtaRegisterObject(ObjectBase *obj);
Window YtaInit(char *title, int width, int height);
void YtaRegisterShader(int key, GLuint prog, void (*bind)(MalerContainer *));
MalerElement *YtaCreate(void *instance, int instance_size, int type,
						Texture *texture);
MalerElement *YtaCreateEx(void *instance, int instance_size, int type,
						  Texture *texture, MalerContainer *container);
MalerContainer *YtaCreateContainer(int shader_type, int texture_id);
void YtaDestroyElement(MalerContainer *container, size_t index);
void YtaDestroy(void);
Mouse YtaGetMouse(void);
Texture *YtaLoadTexture(int id, const char *filename);
Texture *YtaGetTexture(int id);
Window YtaGetWindow(void);
void YtaRun(void);
void YtaClear(Color color);
int YtaShouldClose(void);
double YtaGetTime(void);
float YtaDelta(void);
#endif // YTA_H_
