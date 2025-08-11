#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec2 TxtCoord;

uniform sampler2D inblk;
uniform sampler2D intexture;

void main()
{
vec4 blk_tex = texture(inblk, TexCoord);
vec4 txt_tex = texture(intexture, TxtCoord);
float alph = round(txt_tex.r);
vec3 txtcol = vec3(1.0, 0.0, 0.0);
// vec3(0.16, 0.25, 0.75);
vec4 txcol = vec4(txtcol, alph);

// if(485 < gl_FragCoord.x && gl_FragCoord.x < 510)
//     FragColor = vec4(0.0, 1.0, 0.0, 1.0);
// else if((485 > TxtCoord.x || TxtCoord.x > 510) && txcol.a != 0)
//     FragColor = vec4(0.0, 0.0, 1.0, 1.0);
// else
//     FragColor = blk_tex * (1 - txcol.a) + txcol * txcol.a;

// FragColor = txcol;
FragColor = blk_tex * (1 - txcol.a) + txcol * txcol.a;
}