#include <main.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include <UI/ui.h>

container_t *root, *c1;

void cb_error(int error, const char *description)
{
	fprintf(stderr, "ERROR %d: %s\n", error, description);
}

void cb_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	glfwSetErrorCallback(cb_error);
	if (!glfwInit()) {
		exit(0);
	}

	glfwWindowHint(GLFW_SAMPLES, SAMPLES);

	GLFWwindow *win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Writing App", NULL, NULL);
	glfwMakeContextCurrent(win);

	glfwSetFramebufferSizeCallback(win, cb_resize);
	
	int fbwidth, fbheight;
	glfwGetFramebufferSize(win, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);
	glOrtho(0, fbwidth, fbheight, 0, 0, 1);

	glfwSwapInterval(0);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	ui_init(0, 0, fbwidth, fbheight);

	widget_t *w1 = ui_widget_create(WIDGET_WIDGET);
	w1->border_radius = 50;
	w1->position_in_parent = 0;
	w1->main_color = (color_t) {
		.r = 1.0,
		.g = 0.0,
		.b = 0.0,
		.a = 1.0,
	};

	widget_t *w2 = ui_widget_create(WIDGET_WIDGET);
	w2->border_radius = 100;
	w2->position_in_parent = 0;
	w2->main_color = (color_t) {
		.r = 0.0,
		.g = 1.0,
		.b = 0.0,
		.a = 1.0,
	};

	widget_t *w3 = ui_widget_create(WIDGET_WIDGET);
	w3->position_in_parent = 1;
	w3->main_color = (color_t) {
		.r = 0.0,
		.g = 0.0,
		.b = 1.0,
		.a = 1.0,
	};

	widget_t *w4 = ui_widget_create(WIDGET_WIDGET);
	w4->positioning = POS_RIGHT;
	w4->position_in_parent = 2;
	w4->width = 200;
	w4->height = 200;
	w4->main_color = (color_t) {
		.r = 0.0,
		.g = 1.0,
		.b = 1.0,
		.a = 1.0,
	};

	root = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	root->children_max = 2;
	root->orientation = ORIENTATION_HORIZONTAL;
	root->main_color = (color_t) {
		.r = 0.1,
		.g = 0.1,
		.b = 0.1,
		.a = 1.0,
	};

	c1 = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	c1->children_max = 3;
	c1->position_in_parent = 1;
	c1->positioning = POS_CENTER;
	c1->width = 800;
	c1->height = 800;
	c1->border_radius = 10;
	c1->orientation = ORIENTATION_VERTICAL;
	c1->main_color = (color_t) {
		.r = 0.5,
		.g = 0.5,
		.b = 0.5,
		.a = 1.0,
	};

	ui_widget_set_root(WIDGET(root));
	
	ui_container_add(root, w1);
	ui_container_add(root, WIDGET(c1));
	ui_container_add(c1, w2);
	ui_container_add(c1, w3);
	ui_container_add(c1, w4);


	while(!glfwWindowShouldClose(win)) {
		ui_widget_draw_recursive(WIDGET(root));

		glfwSwapBuffers(win);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
	}


	ui_widget_destroy(w1);
	ui_widget_destroy(w2);
	ui_widget_destroy(WIDGET(root));

	glfwDestroyWindow(win);

	glfwTerminate();
}