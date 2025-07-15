#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoord;

void main()
{
gl_Position = vec4(pos, 1.0);
TexCoord = texcoord;
}