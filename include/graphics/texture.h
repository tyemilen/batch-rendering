#ifndef GRAPHIC_TEXTURE_H_
#define GRAPHIC_TEXTURE_H_

#include "maler.h"
#include "ygl.h"

#define SHADER_TEXTURE 0x4

typedef struct {
	float box[4];
	float color[4];
	float uv[4];
} TextureInstance;

GLuint shader_texture_get(void);
void shader_texture_bind(MalerContainer *);

#endif // GRAPHIC_TEXTURE_H_
