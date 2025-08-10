#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec2 txtcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec2 TxtCoord;

void main()
{
gl_Position = transpose(model * view * projection) * vec4(pos.x, pos.y, 0.0, 1.0);
TexCoord = texcoord;

mat3 flp = {
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
};
mat3 trns = {
    vec3(1.0, 0.0, -0.5),
    vec3(0.0, 1.0, -0.5),
    vec3(0.0, 0.0, 1.0)
};
TxtCoord = txtcoord;
// (transpose(trns * flp) * vec3(txtcoord, 1.0)).xy;
}