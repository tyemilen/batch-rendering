#include "graphics/texture.h"

#include "maler.h"
#include "shader.h"
#include "ygl.h"

#include "graphics/helpers.h"

static const char *vertex_shader = __include_shader("texture.vert");
static const char *fragment_shader = __include_shader("texture.frag");

void shader_texture_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	BIND_VEC4(1, box, TextureInstance);
	BIND_VEC4(2, color, TextureInstance);
	BIND_VEC4(3, uv, TextureInstance);
}

GLuint shader_texture_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}
