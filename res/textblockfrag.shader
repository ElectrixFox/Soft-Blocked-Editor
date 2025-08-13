#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec2 TxtCoord;

uniform sampler2D inblk;
uniform sampler2D intexture;

void main()
{
vec4 blk_tex = texture2D(inblk, TexCoord);
vec4 txt_tex = texture2D(intexture, TxtCoord - vec2(0.5, 0.5));
float alph = floor(txt_tex.r);
vec3 txtcol = vec3(1.0, 0.0, 0.0);
// vec3(0.16, 0.25, 0.75);
vec4 txcol = vec4(txtcol, alph);

FragColor = blk_tex * (1 - txcol.a) + txcol * txcol.a;
}