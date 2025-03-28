#version 330 core

out vec4 frag_color;

in vec4 vcolor;
in vec2 vtex_uv;

uniform sampler2D u_texture;

void main()
{
	frag_color = texture(u_texture, vtex_uv) * vcolor;
}


