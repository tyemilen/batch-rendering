#include "font.h"

#include "core.h"
#include "texture.h"
#include "ygl.h"

#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

Atlas font_create_atlas(unsigned char *font_data, int font_size,
						int atlas_width) {
	Atlas atlas = {0};
	stbtt_fontinfo font;

	if (!stbtt_InitFont(&font, font_data,
						stbtt_GetFontOffsetForIndex(font_data, 0)))
		return atlas;

	atlas.font = font;
	atlas.font_size = font_size;

	int glyph_count =
		ttUSHORT(font.data + font.fontstart +
				 stbtt__find_table(font.data, font.fontstart, "maxp") + 4);
	if (glyph_count == 0) return atlas;

	stbtt_packedchar *chars = malloc(sizeof(stbtt_packedchar) * glyph_count);
	if (!chars) return atlas;

	int min_height = font_size;
	int max_height = font_size * 32;
	int height = max_height;

	unsigned char *pixels = calloc(atlas_width * max_height, 1);
	if (!pixels) {
		free(chars);
		return atlas;
	}

	while (min_height <= max_height) {
		height = (min_height + max_height) / 2;

		stbtt_pack_context ctx;
		if (!stbtt_PackBegin(&ctx, pixels, atlas_width, height, 0, 1, 0))
			break;

		int ok = stbtt_PackFontRange(&ctx, font_data, 0, (float)font_size, 0,
									 glyph_count, chars);
		stbtt_PackEnd(&ctx);

		if (ok) {
			max_height = height - 1;
		} else {
			min_height = height + 1;
		}
	}

	atlas.image.width = atlas_width;
	atlas.image.height = height;
	atlas.image.pixels = pixels;
	atlas.chars = chars;

	atlas.quads = malloc(sizeof(stbtt_aligned_quad) * glyph_count);
	if (atlas.quads) {
		for (int i = 0; i < glyph_count; ++i) {
			float x = 0, y = 0;
			stbtt_GetPackedQuad(chars, atlas.image.width, atlas.image.height, i,
								&x, &y, &atlas.quads[i], 0);
		}
	}

	atlas.texture = texture_create(0, &atlas.image, GL_RED);
	return atlas;
}
