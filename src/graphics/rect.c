#include "graphics/rect.h"

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "shader.h"

static const char *vertex_shader = R"glsl(
#version 330

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec4 aBox;
layout(location = 2) in vec4 aColor;

uniform vec2 uWindowSize;

out vec2 vLocalPos;
out vec4 vColor;
out float vRadius;
out vec2 vSize;

void main() {
	vec2 pos = aBox.xy + aVertex * aBox.zw;
	vec2 ndc = vec2(2.0 * pos.x / uWindowSize.x - 1.0, 1.0 - 2.0 * pos.y / uWindowSize.y);
	
	gl_Position = vec4(ndc, 0.0, 1.0);

	vLocalPos = aVertex * aBox.zw - 0.5 * aBox.zw;
	vColor = aColor;
	vSize = aBox.zw;
}
)glsl";

static const char *fragment_shader = R"glsl(
#version 330

in vec2 vLocalPos;
in vec4 vColor;
in vec2 vSize;

out vec4 FragColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;

void main() {
	vec2 h = 0.5 * vSize;

	vec4 color = vColor;
	if (uUseTexture) {
		vec2 uv = (vLocalPos / h + 1.0) * 0.5;
		vec4 texColor = texture(uTexture, uv);
		color.rgb = texColor.rgb;
		color.a = color.a * texColor.a;
	}

	FragColor = color;
}
)glsl";

void shader_rect_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance),
						  (void *)offsetof(RectInstance, box));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance),
						  (void *)offsetof(RectInstance, color));
	glVertexAttribDivisor(2, 1);
}

GLuint shader_rect_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}
