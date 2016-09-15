#version 330 core

layout (location =0) in vec3 position;
layout (location =1) in vec3 normal;
layout (location =2) in vec2 uvCoord;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	texCoord =uvCoord;
	gl_Position =projection * model * vec4(position, 1.0);
}