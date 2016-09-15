#version 330 core

in vec3 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    //color =vec4(0.0, 0.5, 1.0, 1.0);
    color =texture(ourTexture, vec2(TexCoord.x, 1.0 - TexCoord.y));
}