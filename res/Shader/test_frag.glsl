#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;
uniform float alpha;

void main()
{
	color =vec4(1.0, 1.0, 1.0, 1.0);
}