#ifndef UI_H
#define UI_H

#include <main.h>
#include <stdbool.h>

#define NUM_ROUNDED_EDGES 32

#define WIDGET(x)    ((widget_t *)(x))
#define CONTAINER(x) ((container_t *)(x))
#define BUTTON(x)    ((button_t *)(x))

typedef enum {
	WIDGET_WIDGET,
	WIDGET_EMPTY,
	WIDGET_CONTAINER,
	WIDGET_BUTTON,
} widget_type_t;

typedef struct {
	double r;
	double g;
	double b;
	double a;
} color_t;

typedef enum {
	ORIENTATION_HORIZONTAL,
	ORIENTATION_VERTICAL,
} container_orientation_t;

typedef struct widget widget_t;
struct widget {
	widget_type_t type;
	widget_t *parent;
	double x;
	double y;
	double width;
	double height;
	double border_radius;
	bool visible;
	color_t main_color;
};

typedef struct {
	widget_t;
	container_orientation_t orientation;
	int children_max;
	int children_num;
	widget_t *children[32];
	double spacing;
} container_t;

typedef struct {
	widget_t;
	char *text;
} button_t;


widget_t *ui_widget_create(widget_type_t type);

void ui_widget_set_root(widget_t *widget);

bool ui_widget_draw(widget_t *widget);
bool ui_widget_draw_recursive(widget_t *widget);

bool ui_container_add(container_t *container, widget_t *widget);

void ui_widget_destroy(widget_t *widget);

void ui_init(int x, int y, int width, int height);

#endif