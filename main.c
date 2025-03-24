#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const char* vertex_src = "#version 330 core\n"
	"layout (location= 0) in vec3 apos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(apos, 1.0);\n"
	"}\0";
	
const char* fragment_src = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0, 0.5, 0.2, 1.0);\n"
	"}\0";

GLFWwindow* window = NULL;
unsigned int vbo = 0;
unsigned int vao = 0;
unsigned int program = 0;

void die(const char* msg) {
	perror(msg);
	exit(1);
}

void on_key_event(GLFWwindow* w, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(w, GLFW_TRUE);
}

int main() {
	if(!glfwInit())
		die("failed to init glfw");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "edit-gl", NULL, NULL);
	if(window == NULL)
		die("glfwCreateWindow");
	glfwMakeContextCurrent(window);
	if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
		die("gladLoadGLLoader");

	printf("openGL initialized.\n\tversion: %s\n\trenderer: %s\n\tvendor: %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_RENDERER),
			glGetString(GL_VENDOR));

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, on_key_event);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float vertices[] = {
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		0.0, 0.5, 0.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_src, NULL);
	glCompileShader(vs);
	int suc;
	char infolog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &suc);
	if(suc == 0){
		glGetShaderInfoLog(vs, 512, NULL, infolog);
		die(infolog);
	}
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_src, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &suc);
	if(suc == 0) {
		glGetShaderInfoLog(fs, 512, NULL, infolog);
		die(infolog);
	}
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &suc);
	if(suc == 0) {
		glGetProgramInfoLog(program, 512, NULL, infolog);
		die(infolog);
	}
	glDeleteShader(vs);
	glDeleteShader(fs);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
