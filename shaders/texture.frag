#version 330

in vec2 vLocalPos;
in vec4 vColor;
flat in vec2 vSize;
in vec2 vTexCoord;

out vec4 FragColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;

void main() {
	vec2 h = 0.5 * vSize;

	vec2 uv = (vLocalPos / h + 1.0) * 0.5;
	vec4 texColor = texture(uTexture, vTexCoord);
	FragColor = texColor;
}