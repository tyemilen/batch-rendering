#include "graphics/grid.h"

#include "maler.h"
#include "shader.h"
#include "ygl.h"

#include "graphics/helpers.h"

static const char *vertex_shader = __include_shader("grid.vert");
static const char *fragment_shader = __include_shader("grid.frag");

void shader_grid_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	BIND_VEC4(1, box, GridInstance);
	BIND_VEC4(2, color, GridInstance);
	BIND_VEC4(3, bg_color, GridInstance);
	BIND_FLOAT(4, cell_size, GridInstance);
}

GLuint shader_grid_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}
