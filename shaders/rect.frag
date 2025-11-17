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