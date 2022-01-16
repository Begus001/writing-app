#include <main.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include <UI/ui.h>

void cb_error(int error, const char *description)
{
	fprintf(stderr, "ERROR %d: %s\n", error, description);
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
	
	int fbwidth, fbheight;
	glfwGetFramebufferSize(win, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);
	glOrtho(0, fbwidth, fbheight, 0, 0, 1);

	glfwSwapInterval(1);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	ui_init(0, 0, fbwidth, fbheight);

	widget_t *w1 = ui_widget_create(WIDGET_WIDGET);

	*w1 = (widget_t) {
		.x = 100,
		.y = 100,
		.width = 1000,
		.height = 500,
		.border_radius = 50,
		.main_color = (color_t) {
			.r = 1.0,
			.g = 0.0,
			.b = 0.0,
			.a = 0.5,
		},
	};

	widget_t *w2 = ui_widget_create(WIDGET_WIDGET);

	*w2 = (widget_t) {
		.x = 150,
		.y = 150,
		.width = 1000,
		.height = 500,
		.border_radius = 100,
		.main_color = (color_t) {
			.r = 0.0,
			.g = 1.0,
			.b = 0.0,
			.a = 0.5,
		},
	};

	widget_t *w3 = ui_widget_create(WIDGET_WIDGET);

	*w3 = (widget_t) {
		.border_radius = 0,
		.main_color = (color_t) {
			.r = 0.0,
			.g = 0.0,
			.b = 1.0,
			.a = 1.0,
		},
	};

	widget_t *w4 = ui_widget_create(WIDGET_WIDGET);

	*w4 = (widget_t) {
		.border_radius = 0,
		.main_color = (color_t) {
			.r = 0.0,
			.g = 1.0,
			.b = 1.0,
			.a = 1.0,
		},
	};

	container_t *root = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	*root = (container_t) {
		.children_max = 2,
		.main_color = (color_t) {
			.r = 0.1,
			.g = 0.1,
			.b = 0.1,
			.a = 1.0,
		},
		.orientation = ORIENTATION_HORIZONTAL,
	};
	ui_widget_set_root(WIDGET(root));

	container_t *c1 = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	*c1 = (container_t) {
		.children_max = 3,
		.main_color = (color_t) {
			.r = 0.5,
			.g = 0.5,
			.b = 0.5,
			.a = 1.0,
		},
		.orientation = ORIENTATION_VERTICAL,
	};

	ui_container_add(root, w1);
	// ui_container_add(root, w2);
	ui_container_add(root, c1);
	ui_container_add(c1, w2);
	ui_container_add(c1, w3);
	ui_container_add(c1, w4);

	w2->width -= 200;
	w2->x += 100;
	w2->height -= 100;
	w2->y += 50;

	while(!glfwWindowShouldClose(win)) {
		ui_widget_draw(root);
		ui_widget_draw(w1);
		ui_widget_draw(c1);
		ui_widget_draw(w2);
		ui_widget_draw(w3);
		ui_widget_draw(w4);

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