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

	button_t *button = ui_widget_create(WIDGET_BUTTON);

	GLFWwindow *win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Writing App", NULL, NULL);
	glfwMakeContextCurrent(win);

	glfwGetFramebufferSize(win, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);
	glOrtho(0, fbwidth, fbheight, 0, 0, 1);

	glfwSwapInterval(0);

	while(!glfwWindowShouldClose(win)) {
		
		
		glfwSwapBuffers(win);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
	}

	glfwDestroyWindow(win);

	glfwTerminate();
}