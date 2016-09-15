#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main()
{
	if (texture(tex, UV).r !=0)
		color =vec4(1.0, 1.0, 1.0, texture(tex, UV).r) * vec4(textColor, 1.0);
	else
		color =vec4(0.0, 0.0, 0.0, 0.0);
}