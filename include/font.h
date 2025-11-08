#ifndef FONT_H_
#define FONT_H_

#include "core.h"
#include "stb_truetype.h"
#include "texture.h"

typedef struct Atlas {
	stbtt_fontinfo font;

	stbtt_packedchar *chars;
	stbtt_aligned_quad *quads;

	int font_size;

	Image image;
	Texture *texture;
} Atlas;

Atlas font_create_atlas(unsigned char *font_data, int font_size,
						int atlas_width);

#endif // FONT_H_
