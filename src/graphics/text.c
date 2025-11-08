#include "graphics/text.h"

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "shader.h"

static const char *vertex_shader = R"glsl(
#version 330

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec4 aBox;
layout(location = 2) in vec4 aUV;
layout(location = 3) in vec4 aColor;

uniform vec2 uWindowSize;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
	vec2 pos = aBox.xy + aVertex * aBox.zw;
	vec2 ndc = vec2(2.0 * pos.x / uWindowSize.x - 1.0, 1.0 - 2.0 * pos.y / uWindowSize.y);

	gl_Position = vec4(ndc, 0.0, 1.0);

	vColor = aColor;
	vTexCoord = mix(aUV.xy, aUV.zw, aVertex);
}
)glsl";

static const char *fragment_shader = R"glsl(
#version 330 core

in vec2 vTexCoord;
in vec4 vColor;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
	float alpha = texture(uTexture, vTexCoord).r;
	FragColor = vec4(vColor.rgb, vColor.a * alpha);
}
)glsl";

GLuint shader_text_get(void) {
	return shader_create(vertex_shader, fragment_shader);
}

void shader_text_bind(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TextInstance),
						  (void *)offsetof(TextInstance, box));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TextInstance),
						  (void *)offsetof(TextInstance, uv));
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TextInstance),
						  (void *)offsetof(TextInstance, color));
	glVertexAttribDivisor(3, 1);
}
