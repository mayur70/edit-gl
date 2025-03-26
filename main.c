#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define BATCH_SIZE 100
#define VERTICES_MAX (BATCH_SIZE * 4)
#define INDICES_MAX (BATCH_SIZE * 6)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct Vertex {
	float x, y, z;
	float r, g, b, a;
} Vertex;

typedef struct Renderer {
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;
	unsigned int program;


	Vertex vertices[VERTICES_MAX];
	size_t vertices_len;
}Renderer;

GLFWwindow* window = NULL;
static Renderer renderer = {0};

void die(const char* msg) {
	perror(msg);
	exit(1);
}

char* read_file_from_disk(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if(fp == NULL)
		die("failed to open file");
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buf = malloc(len + 1);
	fread(buf, len, 1, fp);
	buf[len] = '\0';
	return buf;
}

void on_key_event(GLFWwindow* w, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(w, GLFW_TRUE);
}

void renderer_create(Renderer* r) {
	memset(r, 0, sizeof(*r));
	glGenVertexArrays(1, &r->vao);
	glBindVertexArray(r->vao);

	glGenBuffers(1, &r->vbo);
	glGenBuffers(1, &r->ebo);

	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTICES_MAX, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
	unsigned int indices[INDICES_MAX]; 

	for(size_t i = 0, idx = 0; i < INDICES_MAX; i+= 6, idx+=4) {
		indices[i + 0] = idx + 0;
		indices[i + 1] = idx + 1;
		indices[i + 2] = idx + 2;
		indices[i + 3] = idx + 1;
		indices[i + 4] = idx + 3;
		indices[i + 5] = idx + 2;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	char* vertex_src = read_file_from_disk("res/basic.vert");
	char* fragment_src = read_file_from_disk("res/basic.frag");

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
	free(vertex_src);
	free(fragment_src);

	r->program = glCreateProgram();
	glAttachShader(r->program, vs);
	glAttachShader(r->program, fs);
	glLinkProgram(r->program);
	glGetProgramiv(r->program, GL_LINK_STATUS, &suc);
	if(suc == 0) {
		glGetProgramInfoLog(r->program, 512, NULL, infolog);
		die(infolog);
	}
	glDeleteShader(vs);
	glDeleteShader(fs);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0* sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void renderer_destroy(Renderer* r) {
	glDeleteBuffers(1, &r->vbo);
	glDeleteBuffers(1, &r->ebo);
	glDeleteVertexArrays(1, &r->vao);
	glDeleteProgram(r->program);
}

void renderer_push(Vertex* vertices, size_t n) {
	assert(renderer.vertices_len + n < VERTICES_MAX);
	Vertex* dest = &renderer.vertices[renderer.vertices_len];
	memcpy(dest, vertices, sizeof(*vertices) * n);
	renderer.vertices_len += n;
}

void renderer_push_rect(float x, float y, float w, float h) {
	x = ((x / SCREEN_WIDTH) * 2.0) - 1.0;
	y = ((y / SCREEN_HEIGHT) * 2.0) - 1.0;
	w = w / SCREEN_WIDTH;
	h = h / SCREEN_HEIGHT;
	printf("%f, %f, %f, %f\n", x, y, w, h);
	float trx = x + w, try = y + h,
	      brx = x + w, bry = y,
	      tlx = x, tly = y + h,
	      blx = x, bly = y;
	Vertex rect[4];
	rect[0].x = trx;
	rect[0].y = try;
	rect[0].z = 0.0;
	rect[0].r = 1.0;
	rect[0].g = 0.0;
	rect[0].b = 0.0;
	rect[0].a = 1.0;

	rect[1].x = brx;
	rect[1].y = bry;
	rect[1].z = 0.0;
	rect[1].r = 1.0;
	rect[1].g = 0.0;
	rect[1].b = 0.0;
	rect[1].a = 1.0;

	rect[2].x = tlx;
	rect[2].y = tly;
	rect[2].z = 0.0;
	rect[2].r = 1.0;
	rect[2].g = 0.0;
	rect[2].b = 0.0;
	rect[2].a = 1.0;

	rect[3].x = blx;
	rect[3].y = bly;
	rect[3].z = 0.0;
	rect[3].r = 1.0;
	rect[3].g = 0.0;
	rect[3].b = 0.0;
	rect[3].a = 1.0;

	renderer_push(rect, 4);
}

void renderer_draw() {
	glUseProgram(renderer.program);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(*renderer.vertices) * renderer.vertices_len, renderer.vertices);
	glBindVertexArray(renderer.vao);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
	int count = (renderer.vertices_len / 4) * 6;
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void init() {
	if(!glfwInit())
		die("failed to init glfw");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "edit-gl", NULL, NULL);
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
	
	renderer_create(&renderer);
	renderer_push_rect(200, 150, 400, 300);
}

void shutdown() {
	renderer_destroy(&renderer);
	glfwTerminate();
}

int main() {
	init();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer_draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shutdown();
	return 0;
}
