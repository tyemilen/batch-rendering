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