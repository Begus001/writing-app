#include <UI/ui.h>

#include <stdlib.h>
#include <stddef.h>

widget_t *ui_widget_create(widget_type_t type)
{
	widget_t *widget;
	switch (type) {
		case WIDGET_WIDGET:
			widget = malloc(sizeof(widget_t));
			widget->type = WIDGET_WIDGET;
			return widget;
		case WIDGET_EMPTY:
			widget = malloc(sizeof(widget_t));
			widget->type = WIDGET_WIDGET;
			return widget;
		case WIDGET_CONTAINER:
			widget = malloc(sizeof(container_t));
			widget->type = WIDGET_WIDGET;
			return widget;
		case WIDGET_BUTTON:
			widget = malloc(sizeof(button_t));
			widget->type = WIDGET_BUTTON;
			return widget;
		default:
			return NULL;
	}
}

int ui_widget_draw_all(widget_t *widget)
{
	return 0;
}

int ui_container_add(container_t *container, widget_t *widget)
{
	return 0;
}

void ui_widget_destroy(widget_t *widget)
{
	if (!widget)
		return;

	switch (widget->type) {
		case WIDGET_WIDGET:
		case WIDGET_EMPTY:
		case WIDGET_CONTAINER:
		case WIDGET_BUTTON:
			free(widget);
			break;
		default:
			return;
	}
}
