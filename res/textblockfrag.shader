#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D inblk;
uniform sampler2D intexture;

void main()
{
vec4 blk_tex = texture(inblk, TexCoord);
vec4 txt_tex = texture(intexture, TexCoord);
vec4 sampled = vec4(1.0, 1.0, 1.0, txt_tex.x);
vec4 txcol = vec4(1.0, 0.0, 0.0, 1.0) * sampled;
FragColor = blk_tex * (1 - txcol.a) + txcol * txcol.a;
}