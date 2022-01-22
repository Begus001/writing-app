#include <UI/ui.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int fbx, fby, fbwidth, fbheight;
FT_Library ft;
FT_Face face;

void ui_set_framebuffer_dimensions(int x, int y, int w, int h)
{
	fbx = x;
	fby = y;
	fbwidth = w;
	fbheight = h;
}

color_t ui_color_hex(char *hex)
{
	int len = strlen(hex);
	if (!(len == 4 || len == 7) || hex[0] != '#')
		goto invalid_hex;

	char *valid = "0123456789ABCDEFabcdef";
	for (int i = 1; i < len; i++) {
		bool check = false;
		for (int j = 0; j < 22; j++) {
			if (hex[i] == valid[j]) {
				check = true;
				break;
			}
		}
		if (!check)
			goto invalid_hex;
	}

	if (len == 4) {
		if (hex[1] != hex[2] || hex[2] != hex[3] || hex[1] != hex[3])
			goto invalid_hex;

		double color_val = strtol(&hex[2], NULL, 16) / 255.0;

		return (color_t) {
			.r = color_val,
			.g = color_val,
			.b = color_val,
			.a = 1.0,
		};
	} else {
		char tmp[3][3] = {"", "", ""};
		for (int i = 0, j = 1; i < 3; i++, j += 2)
			strncpy(tmp[i], &hex[j], 2);

		return (color_t) {
			.r = strtol(tmp[0], NULL, 16) / 255.0,
			.g = strtol(tmp[1], NULL, 16) / 255.0,
			.b = strtol(tmp[2], NULL, 16) / 255.0,
			.a = 1.0,
		};
	}

invalid_hex:
	ERRMSG(ERR_INVALID_HEX);
	return (color_t){0};
}

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
		ERRMSG(ERR_UNKNOWN_WIDGET);
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
	widget->xpositioning = POS_FILL;
	widget->ypositioning = POS_FILL;
	return ERR_OK;
}

ui_err_t ui_widget_set_text(widget_t *widget, const char *text)
{
	if (!widget->text) {
		widget->text = malloc(strlen(text) + 1);
	} else {
		widget->text = realloc(widget->text, strlen(text) + 1);
	}

	memset(widget->text, 0, strlen(text) + 1);
	strcpy(widget->text, text);

	return ERR_OK;
}

ui_err_t ui_widget_clear_text(widget_t *widget)
{
	if (widget->text)
		free(widget->text);

	widget->text = NULL;

	return ERR_OK;
}

ui_err_t ui_fill_rounded_rect(double x, double y, double w, double h, double r)
{
	if (r > fmin(w, h) / 2.0) {
		WARNMSG(ERR_INVALID_RADIUS);
		r = fmin(w, h) / 2.0;
	}

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

ui_err_t ui_draw_string(double x, double y, double h, const char *s)
{
	for (size_t i = 0; i < strlen(s); i++) {
		FT_Set_Pixel_Sizes(face, 0, h);
		if (FT_Load_Char(face, s[i], FT_LOAD_RENDER)) {
			ERRMSG(ERR_FREETYPE_CHAR);
			return ERR_FREETYPE_CHAR;
		}

		FT_Bitmap bm = face->glyph->bitmap;
		double bearingx = face->glyph->metrics.horiBearingX / 64.0;
		double bearingy = face->glyph->metrics.horiBearingY / 64.0;

		glBegin(GL_POINTS);
		for (unsigned int row = 0; row < bm.rows; row++) {
			for (unsigned int p = 0; p < bm.width; p++) {
				if (bm.buffer[row * bm.width + p])
					glVertex2d(x + p + bearingx, y + row - bearingy);
			}
		}
		glEnd();
		x += face->glyph->metrics.horiAdvance / 64.0;
	}
	return ERR_OK;
}

ui_err_t ui_draw_string_centered(double x, double y, double h, const char *s)
{
	size_t len = strlen(s);
	double width = 0, height = 0;
	for (size_t i = 0; i < len; i++) {
		FT_Set_Pixel_Sizes(face, 0, h);
		if (FT_Load_Char(face, s[i], FT_LOAD_NO_BITMAP)) {
			ERRMSG(ERR_FREETYPE_CHAR);
			return ERR_FREETYPE_CHAR;
		}

		FT_Glyph_Metrics m = face->glyph->metrics;
		width += m.horiAdvance / 64.0;
		height += m.height / 64.0;
	}

	height /= len;

	return ui_draw_string(x - width / 2.0, y + height / 2.0, h, s);
}

ui_err_t ui_widget_draw(widget_t *widget)
{
	color_t c = widget->background_color;
	int ret;
	glColor4d(c.r, c.g, c.b, c.a);
	if (widget->border_radius > 0) {
		ret = ui_fill_rounded_rect( widget->x, widget->y, widget->width, widget->height,widget->border_radius);
		if (ret) ERRMSG(ret);
		} else {
			ret = ui_fill_rect(widget->x, widget->y, widget->width, widget->height);
			if (ret) ERRMSG(ret);
		}
	
	if (widget->text) {
		c = widget->font_color;
		glColor4d(c.r, c.g, c.b, c.a);
		ret = ui_draw_string_centered(widget->x + widget->width / 2.0, widget->y + widget->height / 2.0, widget->font_size, widget->text);
		if (ret) ERRMSG(ret);
	}

	return ERR_OK;
}

ui_err_t ui_widget_draw_recursive(widget_t *widget)
{
	if (widget->type != WIDGET_CONTAINER)
		return ui_widget_draw(widget);
	
	container_t *todo_list[2048] = {0};
	todo_list[0] = CONTAINER(widget);
	int todo_num = 1;
	
	container_t **c = &todo_list[0];

	int res;

	do {
		res = ui_widget_draw(WIDGET(*c));
		if (res) return res;
		for (int i = 0; i < (*c)->children_num; i++) {
			widget_t *current = (*c)->children[i];
			res = ui_widget_draw(current);
			if (res) return res;

			if (current->type == WIDGET_CONTAINER)
				todo_list[todo_num++] = CONTAINER(current);
		}
		c++;
	} while(*c);

	return ERR_OK;
}

ui_err_t ui_container_reset_space_sizes(container_t *container)
{
	int sum = 0;
	int num_custom = 0;
	for (int i = 0; i < container->children_max; i++) {
		if (container->space_size_type[i] == SPACE_SIZE_CUSTOM) {
			sum += container->space_size[i];
			num_custom++;
		}
	}

	for (int i = 0; i < container->children_max; i++) {
		if (container->space_size_type[i] == SPACE_SIZE_AUTO) {
			container->space_size[i] = ((container->orientation ? container->height : container->width) - sum) / (container->children_max - num_custom);
		}
	}
	return ERR_OK;
}

ui_err_t ui_container_arrange_children(container_t *container)
{
	ui_container_reset_space_sizes(container);
	for (int i = 0; i < container->children_num; i++) {
		widget_t *widget = container->children[i];

		if (widget->position_in_parent >= container->children_max) {
			ERRMSG(ERR_INVALID_POSITION_IN_PARENT);
			return ERR_INVALID_POSITION_IN_PARENT;
		}
		
		double ww = widget->width, wh = widget->height;
		int wpip = widget->position_in_parent;
		double cx = container->x, cy = container->y, cw = container->width, ch = container->height;

		double css = container->space_size[i];

		int co = container->orientation;
		double coff = 0;
		for (int j = 0; j < i; j++) {
			coff += container->space_size[j];
		}

		switch (widget->xpositioning) {
		case POS_FILL:
			widget->x = co ? cx : (cx + coff);
			widget->width = co ? cw : css;
			break;
		case POS_CENTER:
			widget->x = co ? (cx + cw / 2 - ww / 2) : (cx + css * (wpip + 0.5) - ww / 2);
			break;
		case POS_LEFT:
			widget->x = co ? cx : (cx + coff);
			break;
		case POS_RIGHT:
			widget->x = co ? (cx + cw - ww) : (cx + coff + css - ww);
			break;
		default:
			ERRMSG(ERR_INVALID_POSITIONING);
			return ERR_INVALID_POSITIONING;
		}

		switch (widget->ypositioning) {
		case POS_FILL:
			widget->y = co ? (cy + coff) : cy;
			widget->height = co ? css : ch;
			break;
		case POS_CENTER:
			widget->y = co ? (cy + coff + css * 0.5 - wh / 2) : (cy + ch / 2 - wh / 2);
			break;
		case POS_TOP:
			widget->y = co ? (cy + coff) : cy;
			break;
		case POS_BOTTOM:
			widget->y = co ? (cy + coff + css - ww) : (cy + ch - wh);
			break;
		default:
			ERRMSG(ERR_INVALID_POSITIONING);
			return ERR_INVALID_POSITIONING;
		}
	}
	return ERR_OK;
}

ui_err_t ui_container_arrange_children_recursive(container_t *container)
{
	container_t *todo_list[2048] = {0};
	todo_list[0] = container;
	int todo_num = 1;
	
	container_t **c = &todo_list[0];

	int res;

	do {
		res = ui_container_arrange_children(*c);
		if (res) return res;
		for (int i = 0; i < (*c)->children_num; i++) {
			if ((*c)->children[i]->type == WIDGET_CONTAINER)
				todo_list[todo_num++] = CONTAINER((*c)->children[i]);
		}
		c++;
	} while(*c);

	return ERR_OK;
}

ui_err_t ui_container_add(container_t *container, widget_t *widget)
{
	if (widget->position_in_parent >= container->children_max) {
		ERRMSG(ERR_INVALID_POSITION_IN_PARENT);
		return ERR_INVALID_POSITION_IN_PARENT;
	} else if (container->children_num >= container->children_max || (unsigned long)container->children_num >= sizeof(container->children)) {
		ERRMSG(ERR_CONTAINER_FULL);
		return ERR_CONTAINER_FULL;
	}
	
	container->children[container->children_num++] = widget;
	ui_err_t err = ui_container_arrange_children(container);
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
		if (widget->text)
			free(widget->text);
		free(widget);
		return ERR_OK;
	default:
		ERRMSG(ERR_UNKNOWN_WIDGET);
		return ERR_UNKNOWN_WIDGET;
	}
}

static char *find_font(const char *name)
{
	char cmd[128];
	int pos = 0;
	char *output = malloc(sizeof(char) * 256);
	memset(output, 0, 256);
	sprintf(cmd, "find %s -name \"%s\"", UI_FONT_SEARCH_DIR, name);

	FILE *pipe = popen(cmd, "r");
	int n;
	do {
		n = fread(&output[pos], 1, 1, pipe);
		if (output[pos] == '\n') {
			output[pos] = '\0';
			break;
		}
	} while (n > 0 && ++pos < 127);

	fclose(pipe);

	return output;
}

ui_err_t ui_init(int x, int y, int width, int height)
{
	fbx = x;
	fby = y;
	fbwidth = width;
	fbheight = height;

	if (FT_Init_FreeType(&ft)) {
		ERRMSG(ERR_FREETYPE_INIT);
		return ERR_FREETYPE_INIT;
	}
	
	char *path = find_font(UI_FONT);
	
	if (FT_New_Face(ft, path, 0, &face)) {
		free(path);
		ERRMSG(ERR_FREETYPE_FACE);
		return ERR_FREETYPE_FACE;
	}

	free(path);

	return ERR_OK;
}

char *ui_err_to_message(ui_err_t err)
{
	switch (err) {
	case ERR_OK:
		return "";
	case ERR_CONTAINER_FULL:
		return "Container is full";
	case ERR_INVALID_POSITIONING:
		return "Widget has invalid attribute \"xpositioning\" or \"ypositioning\"";
	case ERR_INVALID_POSITION_IN_PARENT:
		return "Widget has invalid attribute \"position_in_parent\"";
	case ERR_INVALID_RADIUS:
		return "Rounded rectangle radius cannot be higher than half of the shortest side";
	case ERR_UNKNOWN_WIDGET:
		return "Widget with unknown type received";
	case ERR_INVALID_HEX:
		return "Given value is not a valid hex color code";
	case ERR_FREETYPE_INIT:
		return "Initialization of FreeType failed";
	case ERR_FREETYPE_FACE:
		return "No type face found";
	case ERR_FREETYPE_CHAR:
		return "Couldn't load FreeType character";
	case ERR_NOT_IMPLEMENTED:
		return "The requested functionality has not been implemented yet";
	default:
		return "Unknown error type";
	}
}
