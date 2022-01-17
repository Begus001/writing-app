#include <UI/ui.h>

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
		CONTAINER(widget)->children_max = 2;
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

ui_err_t ui_widget_set_root(widget_t *widget)
{
	widget->x = fbx;
	widget->y = fby;
	widget->width = fbwidth;
	widget->height = fbheight;
	widget->border_radius = 0;
	widget->parent = NULL;
	widget->position_in_parent = 0;
	widget->positioning = POS_FILL;
	return ERR_OK;
}

ui_err_t ui_fill_rounded_rect(double x, double y, double w, double h, double r)
{
	if (r > fmin(w, h) / 2.0)
		return ERR_INVALID_RADIUS;

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
	return ERR_OK;
}

ui_err_t ui_fill_rect(double x, double y, double w, double h)
{
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y + h);
	glVertex2d(x, y + h);
	glEnd();
	return ERR_OK;
}

ui_err_t ui_widget_draw(widget_t *widget)
{
	color_t c = widget->main_color;
	glColor4d(c.r, c.g, c.b, c.a);
	if (widget->border_radius > 0)
		return ui_fill_rounded_rect(
			widget->x, widget->y, widget->width, widget->height, widget->border_radius);
	else
		return ui_fill_rect(widget->x, widget->y, widget->width, widget->height);
}

// ui_err_t ui_widget_draw_recursive(widget_t *widget)
// {
//
// }

ui_err_t ui_container_reset_positioning(container_t *container)
{
	container->space_size = (container->orientation ? container->height : container->width) / container->children_max;

	for (int i = 0; i < container->children_num; i++) {
		widget_t *widget = container->children[i];

		if (widget->position_in_parent >= container->children_max)
			return ERR_INVALID_POSITION_IN_PARENT;

		switch (widget->positioning) {
		case POS_FILL:
			widget->x = container->orientation ? container->x : (container->x + container->space_size * widget->position_in_parent);
			widget->y = container->orientation ? (container->y + container->space_size * widget->position_in_parent) : container->y;
			widget->width = container->orientation ? container->width : container->space_size;
			widget->height = container->orientation ? container->space_size : container->height;
			break;
		default:
			return ERR_NOT_IMPLEMENTED;
		}
	}
	return ERR_OK;
}

ui_err_t ui_container_add(container_t *container, widget_t *widget)
{
	if (widget->position_in_parent >= container->children_max)
		return ERR_INVALID_POSITION_IN_PARENT;
	else if (container->children_num >= container->children_max || (unsigned long)container->children_num >= sizeof(container->children))
		return ERR_CONTAINER_FULL;
	
	container->children[container->children_num++] = widget;
	ui_err_t err = ui_container_reset_positioning(container);
	if (err) {
		container->children_num--;
		return err;
	}
	widget->parent = WIDGET(container);
	return ERR_OK;
}

ui_err_t ui_widget_destroy(widget_t *widget)
{
	if (!widget)
		return ERR_OK;

	switch (widget->type) {
	case WIDGET_WIDGET:
	case WIDGET_CONTAINER:
	case WIDGET_BUTTON:
		free(widget);
		return ERR_OK;
	default:
		return ERR_UNKNOWN_WIDGET;
	}
}

ui_err_t ui_init(int x, int y, int width, int height)
{
	fbx = x;
	fby = y;
	fbwidth = width;
	fbheight = height;
	return ERR_OK;
}

char *ui_err_to_message(ui_err_t err)
{
	switch (err) {
	case ERR_OK:
		return "OK";
	case ERR_CONTAINER_FULL:
		return "Container is full";
	case ERR_INVALID_POSITION_IN_PARENT:
		return "Widget has invalid attribute \"position_in_parent\"";
	case ERR_INVALID_RADIUS:
		return "Rounded rectangle radius cannot be higher than half of the shortest side";
	case ERR_UNKNOWN_WIDGET:
		return "Widget with unknown type received";
	case ERR_NOT_IMPLEMENTED:
		return "The requested functionality has not been implemented yet";
	default:
		return "Unknown error type";
	}
}
