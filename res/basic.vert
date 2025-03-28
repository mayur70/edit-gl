#version 330 core
layout (location= 0) in vec3 apos;
layout (location = 1) in vec4 acolor;
layout (location = 2) in vec2 atex_uv;

out vec4 vcolor;
out vec2 vtex_uv;

void main()
{
	gl_Position = vec4(apos, 1.0);
	vcolor = acolor;
	vtex_uv = atex_uv;
}


