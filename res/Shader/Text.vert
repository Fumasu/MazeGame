#version 330 core

layout (location =0) in vec3 position;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 uvCoord;

uniform mat4 projection;

out vec2 UV;

void main()
{
	UV =uvCoord;
	gl_Position =projection * vec4(position, 1.0);
}