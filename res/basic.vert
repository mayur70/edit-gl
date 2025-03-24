#version 330 core
layout (location= 0) in vec3 apos;
layout (location = 1) in vec4 acolor;

out vec4 vcolor;

void main()
{
	gl_Position = vec4(apos, 1.0);
	vcolor = acolor;
}


