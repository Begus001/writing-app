#include <main.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include <UI/ui.h>

GLFWwindow *win;
container_t *root;

void cb_error(int error, const char *description)
{
	fprintf(stderr, "ERROR %d: %s\n", error, description);
}

void cb_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);

	ui_set_framebuffer_dimensions(0, 0, width, height);
	ui_widget_set_root(WIDGET(root));
	ui_container_arrange_children_recursive(root);

	glClear(GL_COLOR_BUFFER_BIT);
	ui_widget_draw_recursive(WIDGET(root));
	glColor3d(1, 1, 1);
	ui_draw_char(100, 100, 0.25 * (height + width), 'X');
	glfwSwapBuffers(win);
	ui_widget_draw_recursive(WIDGET(root));
	ui_draw_char(100, 100, 0.25 * (height + width), 'X');
}

void cb_maximize(GLFWwindow* window, int maximized)
{
	glClear(GL_COLOR_BUFFER_BIT);
	ui_widget_draw_recursive(WIDGET(root));
	glColor3d(1, 1, 1);
	ui_draw_char(100, 100, 100, 'X');
	glfwSwapBuffers(win);
	ui_widget_draw_recursive(WIDGET(root));
	ui_draw_char(100, 100, 100, 'X');
}

int main(void)
{
	glfwSetErrorCallback(cb_error);
	if (!glfwInit()) {
		exit(EXIT_GLFW_INIT_ERR);
	}

	glfwWindowHint(GLFW_SAMPLES, SAMPLES);

	win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Writing App", NULL, NULL);
	glfwMakeContextCurrent(win);

	glfwSetFramebufferSizeCallback(win, cb_resize);
	glfwSetWindowMaximizeCallback(win, cb_maximize);
	
	int fbwidth, fbheight;
	glfwGetFramebufferSize(win, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, fbwidth, fbheight, 0, 0, 1);

	glfwSwapInterval(0);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	ui_err_t ret = ui_init(0, 0, fbwidth, fbheight);
	if (ret) {
		ERRMSG(ret);
		exit(EXIT_UI_INIT_ERR);
	}

	root = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	root->main_color = ui_color_hex("#000000");
	ui_widget_set_root(WIDGET(root));
	ui_widget_draw_recursive(WIDGET(root));
	glColor3d(1, 1, 1);
	ui_draw_char(100, 100, 100, 'X');
	glfwSwapBuffers(win);
	ui_widget_draw_recursive(WIDGET(root));
	ui_draw_char(100, 100, 100, 'X');


	while(!glfwWindowShouldClose(win)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(win);

	glfwTerminate();
}