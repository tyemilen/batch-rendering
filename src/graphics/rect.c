#include "graphics/rect.h"

#include <stddef.h>

#include "maler.h"
#include "shader.h"
#include "ygl.h"

#include "graphics/helpers.h"

static const char *vertex_shader = __include_shader("rect.vert");
static const char *fragment_shader = __include_shader("rect.frag");

void shader_rect_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	BIND_VEC4(1, box, RectInstance);
	BIND_VEC4(2, color, RectInstance);
}

GLuint shader_rect_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}
