#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec2 txtcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec2 TxtCoord;

vec2 modCoord(vec2 inc)
{
mat3 flp = {
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
};
mat3 trns = {
    vec3(2.0, 0.0, 0.0),
    vec3(0.0, 2.0, 0.0),
    vec3(0.0, 0.0, 1.0)
};

return (vec3(inc, 1.0) * trns).xy;
}

void main()
{
gl_Position = transpose(model * view * projection) * vec4(pos.x, pos.y, 1.0, 1.0);
TexCoord = texcoord;

vec2 vcs[] = {
    vec2(1.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0),
    vec2(0.0, 1.0)
};
TxtCoord = modCoord(vcs[gl_VertexID]);
}