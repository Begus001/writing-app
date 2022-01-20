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

	// glClear(GL_COLOR_BUFFER_BIT);
	// ui_widget_draw_recursive(WIDGET(root));
	// glfwSwapBuffers(win);
}

void cb_maximize(GLFWwindow* window, int maximized)
{
	// glClear(GL_COLOR_BUFFER_BIT);
	// ui_widget_draw_recursive(WIDGET(root));
	// glfwSwapBuffers(win);
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
	root->main_color = ui_color_hex("#222");
	root->orientation = ORIENTATION_VERTICAL;
	root->children_max = 2;
	root->space_size_type[0] = SPACE_SIZE_CUSTOM;
	root->space_size[0] = 100;

	ui_widget_set_root(WIDGET(root));
	
	container_t *top_bar = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	top_bar->children_max = 3;
	top_bar->main_color = ui_color_hex("#111");
	top_bar->orientation = ORIENTATION_HORIZONTAL;
	top_bar->position_in_parent = 0;

	widget_t *content = ui_widget_create(WIDGET_WIDGET);
	content->main_color = ui_color_hex("#555");
	content->border_radius = 100;
	content->position_in_parent = 1;
	content->xpositioning = content->ypositioning = POS_CENTER;
	content->width = 800;
	content->height = 500;

	glClear(GL_COLOR_BUFFER_BIT);
	ui_container_add(root, WIDGET(top_bar));
	ui_container_add(root, content);

	ui_widget_draw_recursive(WIDGET(root));

	glfwSwapBuffers(win);

	while(!glfwWindowShouldClose(win)) {
		glClear(GL_COLOR_BUFFER_BIT);
		ui_widget_draw_recursive(WIDGET(root));
		glColor3d(1, 1, 1);
		ui_draw_string(root->width / 2.0 - 185, root->height / 2.0 + 80, 100, "Content");
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwDestroyWindow(win);

	glfwTerminate();
}