#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

GLFWwindow* window = NULL;

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

	window = glfwCreateWindow(800, 600, "edit-gl", NULL, NULL);
	if(window == NULL)
		die("glfwCreateWindow");
	glfwMakeContextCurrent(window);
	if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
		die("gladLoadGLLoader");

	glfwSetKeyCallback(window, on_key_event);
	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
