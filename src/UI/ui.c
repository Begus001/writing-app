#include <UI/ui.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <GLFW/glfw3.h>


int fbx, fby, fbwidth, fbheight;

widget_t *ui_widget_create(widget_type_t type)
{
	widget_t *widget;
	switch (type) {
		case WIDGET_WIDGET:
			widget = malloc(sizeof(widget_t));
			memset(widget, 0, sizeof(widget_t));
			widget->type = WIDGET_WIDGET;
			break;
		case WIDGET_CONTAINER:
			widget = malloc(sizeof(container_t));
			memset(widget, 0, sizeof(container_t));
			widget->type = WIDGET_CONTAINER;
			break;
		case WIDGET_BUTTON:
			widget = malloc(sizeof(button_t));
			memset(widget, 0, sizeof(button_t));
			widget->type = WIDGET_BUTTON;
			break;
		default:
			return NULL;
	}

	widget->visible = true;
	return widget;
}

void ui_widget_set_root(widget_t *widget)
{
	widget->x = 0;
	widget->y = 0;
	widget->width = fbwidth;
	widget->height = fbheight;
	widget->border_radius = 0;
	widget->parent = NULL;
}

void ui_fill_rounded_rect(double x, double y, double w, double h, double r)
{
	if (r > fmin(w, h) / 2.0)
		r = fmin(w, h) / 2;

	glBegin(GL_POLYGON);
	for (double d = 0.0; d <= M_PI_2; d += M_PI_2 / NUM_ROUNDED_EDGES)
		glVertex2d(x + r * (1 - cos(d)), y + r * (1 - sin(d)));
	for (double d = 0.0; d <= M_PI_2; d += M_PI_2 / NUM_ROUNDED_EDGES)
		glVertex2d(x + w - r * (1 - sin(d)), y + r * (1 - cos(d)));
	for (double d = 0.0; d <= M_PI_2; d += M_PI_2 / NUM_ROUNDED_EDGES)
		glVertex2d(x + w - r * (1 - cos(d)), y + h - r * (1 - sin(d)));
	for (double d = 0.0; d <= M_PI_2; d += M_PI_2 / NUM_ROUNDED_EDGES)
		glVertex2d(x + r * (1 - sin(d)), y + h - r * (1 - cos(d)));
	glEnd();
}

void ui_fill_rect(double x, double y, double w, double h)
{
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y + h);
	glVertex2d(x, y + h);
	glEnd();
}

void ui_widget_draw(widget_t *widget)
{
	color_t c = widget->main_color;
	glColor4d(c.r, c.g, c.b, c.a);
	if (widget->border_radius > 0)
		ui_fill_rounded_rect(widget->x, widget->y, widget->width, widget->height, widget->border_radius);
	else
		ui_fill_rect(widget->x, widget->y, widget->width, widget->height);
}

// void ui_widget_draw_recursive(widget_t *widget)
// {
//
// }

void ui_container_add(container_t *container, widget_t *widget)
{
	container->children[container->children_num] = widget;
	if (container->orientation == ORIENTATION_HORIZONTAL) {
		widget->x = (container->width / container->children_max) * container->children_num;
		widget->y = container->y;
		widget->width = container->width / container->children_max;
		widget->height = container->height;
	} else {
		widget->x = container->x;
		widget->y = (container->height / container->children_max) * container->children_num;
		widget->width = container->width;
		widget->height = container->height / container->children_max;
	}

	container->children_num++;
}

void ui_widget_destroy(widget_t *widget)
{
	if (!widget)
		return;

	switch (widget->type) {
		case WIDGET_WIDGET:
		case WIDGET_CONTAINER:
		case WIDGET_BUTTON:
			free(widget);
			break;
		default:
			return;
	}
}

void ui_init(int x, int y, int width, int height)
{
	fbx = x;
	fby = y;
	fbwidth = width;
	fbheight = height;
}
