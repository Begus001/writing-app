#ifndef UI_H
#define UI_H

#include <main.h>
#include <stdbool.h>

#define NUM_ROUNDED_EDGES 32

#define UI_FONT "arial.ttf"
#define UI_FONT_SEARCH_DIR "/usr/share/fonts"

#define WIDGET(x)    ((widget_t *)(x))
#define CONTAINER(x) ((container_t *)(x))
#define BUTTON(x)    ((button_t *)(x))

#define ERRMSG(x)    (fprintf(stderr, "UI Error: %s\n", ui_err_to_message((x))))
#define WARNMSG(x)   (fprintf(stderr, "UI Warning: %s\n", ui_err_to_message((x))))


typedef enum {
	ERR_OK,
	ERR_CONTAINER_FULL,
	ERR_INVALID_POSITIONING,
	ERR_INVALID_POSITION_IN_PARENT,
	ERR_INVALID_RADIUS,
	ERR_UNKNOWN_WIDGET,
	ERR_INVALID_HEX,
	ERR_FREETYPE_INIT,
	ERR_FREETYPE_FACE,
	ERR_FREETYPE_CHAR,
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
	POS_LEFT,
	POS_TOP,
	POS_RIGHT,
	POS_BOTTOM,
} widget_positioning_t;

typedef enum {
	SPACE_SIZE_AUTO,
	SPACE_SIZE_CUSTOM,
} container_space_size_t;

typedef struct widget widget_t;

struct widget {
	widget_type_t type;
	widget_t *parent;
	double x;
	double y;
	double width;
	double height;
	widget_positioning_t xpositioning;
	widget_positioning_t ypositioning;
	double border_radius;
	bool visible;
	int position_in_parent;
	color_t background_color;
	color_t font_color;
	int font_size;
	char *text;
};

typedef struct {
	widget_t;
	container_orientation_t orientation;
	container_space_size_t space_size_type[1024];
	double space_size[1024];
	double spacing;
	int children_max;
	int children_num;
	widget_t *children[1024];
} container_t;

// Compatible with GLFW mouse button type
typedef enum {
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_BACKWARD,
	MOUSE_BUTTON_FORWARD,
} mouse_button_t;

// Compatible with GLFW actions
typedef enum {
	ACTION_RELEASE,
	ACTION_PRESS,
} mouse_action_t;

typedef struct {
	widget_t;
	void (*cb_click)(widget_t *widget, mouse_button_t mb, mouse_action_t action);
} button_t;

extern widget_t *root_widget;

void ui_set_framebuffer_dimensions(int x, int y, int w, int h);

color_t ui_color_hex(char *hex);

widget_t *ui_widget_create(widget_type_t type);

ui_err_t ui_widget_set_root(widget_t *widget);
ui_err_t ui_widget_set_text(widget_t *widget, const char *text);
ui_err_t ui_widget_clear_text(widget_t *widget);

ui_err_t ui_fill_rounded_rect(double x, double y, double w, double h, double r);
ui_err_t ui_fill_rect(double x, double y, double w, double h);
ui_err_t ui_draw_string(double x, double y, double h, const char *s);
ui_err_t ui_draw_string_centered(double x, double y, double h, const char *s);

ui_err_t ui_widget_draw(widget_t *widget);
ui_err_t ui_widget_draw_recursive(widget_t *widget);
ui_err_t ui_widget_draw_all(void);

ui_err_t ui_container_reset_space_sizes(container_t *container);
ui_err_t ui_container_arrange_children(container_t *container);
ui_err_t ui_container_arrange_children_recursive(container_t *container);
ui_err_t ui_container_add(container_t *container, widget_t *widget);

ui_err_t ui_check_click_handlers(double x, double y, mouse_button_t mb, mouse_action_t action);

ui_err_t ui_widget_destroy(widget_t *widget);

ui_err_t ui_init(int x, int y, int width, int height);

char *ui_err_to_message(ui_err_t err);

#endif