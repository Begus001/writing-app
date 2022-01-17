#ifndef UI_H
#define UI_H

#include <main.h>
#include <stdbool.h>

#define NUM_ROUNDED_EDGES 32

#define WIDGET(x)    ((widget_t *)(x))
#define CONTAINER(x) ((container_t *)(x))
#define BUTTON(x)    ((button_t *)(x))

#define ERRMSG(x)    (fprintf(stderr, "%s\n", ui_err_to_message((x))))


typedef enum {
	ERR_OK,
	ERR_CONTAINER_FULL,
	ERR_INVALID_POSITION_IN_PARENT,
	ERR_INVALID_RADIUS,
	ERR_UNKNOWN_WIDGET,
	ERR_NOT_IMPLEMENTED,
} ui_err_t;

typedef enum {
	WIDGET_WIDGET,
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

typedef enum {
	POS_FILL,
	POS_CENTER,
	POS_CENTER_VERT,
	POS_CENTER_HOR,
} widget_positioning_t;

typedef struct widget widget_t;
struct widget {
	widget_type_t type;
	widget_t *parent;
	double x;
	double y;
	double width;
	double height;
	widget_positioning_t positioning;
	double border_radius;
	bool visible;
	int position_in_parent;
	color_t main_color;
};

typedef struct {
	widget_t;
	container_orientation_t orientation;
	double space_size;
	double spacing;
	int children_max;
	int children_num;
	widget_t *children[1024];
} container_t;

typedef struct {
	widget_t;
	char *text;
} button_t;


widget_t *ui_widget_create(widget_type_t type);

ui_err_t ui_widget_set_root(widget_t *widget);

ui_err_t ui_fill_rounded_rect(double x, double y, double w, double h, double r);
ui_err_t ui_fill_rect(double x, double y, double w, double h);

ui_err_t ui_widget_draw(widget_t *widget);
ui_err_t ui_widget_draw_recursive(widget_t *widget);

ui_err_t ui_container_add(container_t *container, widget_t *widget);

ui_err_t ui_widget_destroy(widget_t *widget);

ui_err_t ui_init(int x, int y, int width, int height);

char *ui_err_to_message(ui_err_t err);

#endif