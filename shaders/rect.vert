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