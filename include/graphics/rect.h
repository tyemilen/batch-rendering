#ifndef GRAPHIC_RECT_H_
#define GRAPHIC_RECT_H_

#define SHADER_RECT 0x1

const char *vertex_shader_rect = R"glsl(
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

const char *fragment_shader_rect = R"glsl(
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

#endif // GRAPHIC_RECT_H_
