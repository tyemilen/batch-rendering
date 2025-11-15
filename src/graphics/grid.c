#include "graphics/grid.h"

#include <stddef.h>

#include "maler.h"
#include "shader.h"
#include "ygl.h"

#include "graphics/helpers.h"

static const char *vertex_shader = R"glsl(
#version 330

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec4 aBox;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec4 aBgColor;
layout(location = 4) in float aCellSize;

uniform vec2 uWindowSize;

out vec2 vLocalPos;
out vec4 vColor;
out vec2 vSize;
out float vCellSize;
out vec4 vBgColor;

void main() {
	vec2 pos = aBox.xy + aVertex * aBox.zw;
	vec2 ndc = vec2(2.0 * pos.x / uWindowSize.x - 1.0, 1.0 - 2.0 * pos.y / uWindowSize.y);
	
	gl_Position = vec4(ndc, 0.0, 1.0);

	vLocalPos = aVertex * aBox.zw - 0.5 * aBox.zw;
	vColor = aColor;
	vSize = aBox.zw;
	vCellSize = aCellSize;
	vBgColor = aBgColor;
}
)glsl";

static const char *fragment_shader = R"glsl(
#version 330

in vec2 vLocalPos;
in vec4 vColor;
in vec2 vSize;
in float vCellSize;
in vec4 vBgColor;

out vec4 FragColor;

void main() {
	vec2 uv = vLocalPos + 0.5 * vSize;
	vec2 posInCell = mod(uv, vCellSize);
	float lineWidth = 1.0;

	float line = 0.0;
	if (posInCell.x < lineWidth || posInCell.y < lineWidth) {
		line = 1.0;
	}

	FragColor = mix(vBgColor, vColor, line);
}
)glsl";

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
