#include <main.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include <UI/ui.h>

GLFWwindow *win;
container_t *root;

double cursorx, cursory;

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
}

void cb_maximize(GLFWwindow* window, int maximized)
{
	
}

void cb_mouse_click(GLFWwindow* window, int button, int action, int mods)
{
	ui_check_click_handlers(cursorx, cursory, button, action);
}

void cb_cursor_pos(GLFWwindow* window, double xpos, double ypos)
{
	cursorx = xpos;
	cursory = ypos;
}

void cb_content_click(widget_t *widget, mouse_button_t mb, mouse_action_t action)
{
	if (action == ACTION_PRESS) {
		printf("Content clicked with button %d!\n", mb);
	} else {
		printf("Content click with button %d released!\n", mb);
	}
	fflush(stdout);
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
	glfwSetMouseButtonCallback(win, cb_mouse_click);
	glfwSetCursorPosCallback(win, cb_cursor_pos);
	
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
	root->background_color = ui_color_hex("#222");
	root->orientation = ORIENTATION_VERTICAL;
	root->children_max = 2;
	root->space_size_type[0] = SPACE_SIZE_CUSTOM;
	root->space_size[0] = 100;

	ui_widget_set_root(WIDGET(root));
	
	container_t *top_bar = CONTAINER(ui_widget_create(WIDGET_CONTAINER));
	top_bar->children_max = 3;
	top_bar->background_color = ui_color_hex("#111");
	top_bar->orientation = ORIENTATION_HORIZONTAL;
	top_bar->position_in_parent = 0;
	ui_widget_set_text(WIDGET(top_bar), "Top Bar");
	top_bar->font_color = ui_color_hex("#FFF");
	top_bar->font_size = 75;

	button_t *content = BUTTON(ui_widget_create(WIDGET_BUTTON));
	content->background_color = ui_color_hex("#555");
	content->border_radius = 100;
	content->position_in_parent = 1;
	content->xpositioning = content->ypositioning = POS_CENTER;
	content->width = 800;
	content->height = 500;
	ui_widget_set_text(WIDGET(content), "Content");
	content->font_color = ui_color_hex("#FFF");
	content->font_size = 100;
	content->cb_click = cb_content_click;

	glClear(GL_COLOR_BUFFER_BIT);
	ui_container_add(root, WIDGET(top_bar));
	ui_container_add(root, WIDGET(content));

	ui_widget_draw_all();

	glfwSwapBuffers(win);

	while(!glfwWindowShouldClose(win)) {
		glClear(GL_COLOR_BUFFER_BIT);
		ui_widget_draw_all();
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	ui_widget_destroy(WIDGET(root));
	ui_widget_destroy(WIDGET(top_bar));
	ui_widget_destroy(WIDGET(content));

	glfwDestroyWindow(win);

	glfwTerminate();
}