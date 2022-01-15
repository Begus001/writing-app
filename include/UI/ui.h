#ifndef UI_H
#define UI_H

#include <main.h>
#include <stdbool.h>

typedef enum {
	WIDGET_WIDGET,
	WIDGET_EMPTY,
	WIDGET_CONTAINER,
	WIDGET_BUTTON,
} widget_type_t;

typedef enum {
	ORIENTATION_HORIZONTAL,
	ORIENTATION_VERTICAL,
} container_orientation_t;

typedef struct widget widget_t;
struct widget{
	widget_type_t type;
	widget_t *parent;
	double width;
	double height;
	bool visible;
};

typedef struct {
	widget_t;
	container_orientation_t orientation;
	int children_num;
	widget_t children[32];
	double spacing;
} container_t;

typedef struct {
	widget_t;
	char *text;
} button_t;


widget_t *ui_widget_create(widget_type_t type);

int ui_widget_draw_all(widget_t *widget);
int ui_container_add(container_t *container, widget_t *widget);

void ui_widget_destroy(widget_t *widget);

#endif