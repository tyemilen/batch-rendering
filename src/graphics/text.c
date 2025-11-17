#include "graphics/text.h"

#include <stddef.h>

#include "maler.h"
#include "shader.h"
#include "ygl.h"

#include "graphics/helpers.h"

static const char *vertex_shader = __include_shader("text.vert");
static const char *fragment_shader = __include_shader("text.frag");

void shader_text_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	BIND_VEC4(1, box, TextInstance);
	BIND_VEC4(2, uv, TextInstance);
	BIND_VEC4(3, color, TextInstance);
}

GLuint shader_text_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}
