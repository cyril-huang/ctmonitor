#include <unistd.h>
#include "ctm.h"
#include "draw_graph.h"

#define CANVAS_TO_CAIRO(x) (x + CANVAS_PADDING)
#define TEMP_TO_CAIRO(T, Tmax, canvas_height) \
	(CANVAS_TO_CAIRO(canvas_height - canvas_height * T / Tmax))
#define TIME_TO_CAIRO(t, tmax, canvas_width) \
	(CANVAS_TO_CAIRO((tmax - t) * canvas_width / tmax))

extern struct ct_info *ct_core;
extern struct system_ct_info system_ct;
extern int ncore;
static int ct_label_height;

static void bg_clean(GObject *bg)
{
	g_object_unref(bg);
}

static void bg_draw(GtkWidget *cairo_widget, cairo_t *cr, struct coordinate *canvas)
{

	double dash[] = { 3.0 };
	int i, interval, n_interval;
	int time_max = SAMPLING_MSECOND;
	int temp_max = system_ct.max;

	cairo_rectangle(cr, canvas->x0, canvas->y0, canvas->width, canvas->height);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_fill(cr);

	/* draw dash grid */
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_set_line_width(cr, 0.5);
	cairo_set_dash(cr, dash, 1, 0);

	n_interval = canvas->height / GRID_TEMP_MIN_INTERVAL;
	if (n_interval > 5) n_interval = 5;
	interval = canvas->height / n_interval;
	for (i = 0; i <= n_interval; i++) {
		int y = CANVAS_TO_CAIRO(i * interval);
		char label[10];
		cairo_move_to(cr, canvas->x0 - 20, y);
		sprintf(label, "%d", (temp_max - i * temp_max / n_interval) / 1000);
		cairo_show_text(cr, label);
		cairo_move_to(cr, canvas->x0, y);
		cairo_line_to(cr, canvas->x0 + canvas->width, y);
	}
	interval = canvas->width / GRID_TIME_NUM_INTERVAL;
	for (i = 0; i <= GRID_TIME_NUM_INTERVAL; i++) {
		int x = CANVAS_TO_CAIRO(i * interval);
		char label[10];
		cairo_move_to(cr, x, canvas->y0);
		cairo_line_to(cr, x, canvas->y0 + canvas->height);
		cairo_move_to(cr, x, canvas->y0 + canvas->height + 20);
		sprintf(label, "%d", (time_max - i * time_max / GRID_TIME_NUM_INTERVAL) / 1000);
		cairo_show_text(cr, label);
	}
	cairo_stroke(cr);
}

static void ct_label_draw(cairo_t *cr, struct coordinate *canvas, int cid)
{
	char label[20];
	int tail = ct_core[cid].ct->tail;
	int row = cid / CT_LABEL_NUM_PER_ROW;
	int col = cid % CT_LABEL_NUM_PER_ROW;
	int x = canvas->x0 + col * canvas->width / CT_LABEL_NUM_PER_ROW;
	int y = canvas->y0 + canvas->height + 40 + row * CT_LABEL_HEIGHT_PER_ROW;

	cairo_rectangle(cr, x, y, CT_LABEL_BRICK_WIDTH, CT_LABEL_BRICK_HEIGHT);
	cairo_fill(cr);
	cairo_move_to(cr, x + CT_LABEL_BRICK_WIDTH + 5, y);
	sprintf(label, "%s:", ct_core[cid].label);
	cairo_show_text(cr, label);
	cairo_move_to(cr, x + CT_LABEL_BRICK_WIDTH + 5, y + 10);
	sprintf(label, "%d", ct_core[cid].ct->data[tail] / 1000);
	cairo_show_text(cr, label);
}

static void ct_draw(cairo_t *cr, struct coordinate *canvas, int t, int temp)
{
	double current_x, current_y;
	double x1, y1, x2, y2, x3, y3;
	int temp_max = system_ct.max;
	int time_max = SAMPLING_MSECOND;
	int dt = SLICE_SECOND MS + SLICE_MSECOND;

	cairo_get_current_point(cr, &current_x, &current_y);

	x3 = TIME_TO_CAIRO(t, time_max, canvas->width);
	y3 = TEMP_TO_CAIRO(temp, temp_max, canvas->height);
	x1 = current_x + (x3 - current_x) / 3;
	y1 = current_y + (y3 - current_y) * 5/ 9;
	x2 = current_x + (x3 - current_x) * 2 / 3;
	y2 = current_y + (y3 - current_y) * 7 / 9;

	cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);
}

struct rgb rgb[] = {
	{1, 0.6, 0},{1, 0.4, 0},{1, 0, 0},
	{0, 0.8, 0},{0, 0.6, 0},{0, 0.4, 0},{0, 0.2, 0},
	{0, 0.6, 1},{0, 0.4, 1},{0, 0.2, 1},{0, 0, 1},
	{0.4, 0, 0},{0.4, 0, 0.2},{0.4, 0.2, 1},{0.4, 0.2, 0.2},
	{0.6, 0, 0.4},{0.6, 0, 0.6},{0.6, 0, 0.8},{0.6, 0, 1},
	{0, 0, 0},{0.13, 0.13, 0.13},{0.33, 0.33, 0.33},{0.4, 0.4, 0.4}
};

void init_ctm_draw()
{
	ct_label_height =
		INT_ROUND(ncore, CT_LABEL_NUM_PER_ROW)
		* CT_LABEL_HEIGHT_PER_ROW;
	return;
}

#ifdef GTK_3
gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer gdata)
#else
gboolean draw_graph(GtkWidget *widget, GdkEventExpose *event, gpointer gdata)
#endif
{
	/* TODO : only the size of window changed need to do the bg_draw */
	struct coordinate cairo, canvas;
	int temp_max = system_ct.max;
	int time_max = SAMPLING_MSECOND;
	double dash[] = {0.3};
	int i;

#ifdef GTK_3
	cairo.width = gtk_widget_get_allocated_width(widget);
	cairo.height = gtk_widget_get_allocated_height(widget);
#else
	cairo_t *cr = gdk_cairo_create(widget->window);
	gdk_drawable_get_size(widget->window, &cairo.width, &cairo.height);

#endif

	cairo.x0 = cairo.y0 = 0;
	canvas.x0 = canvas.y0 = CANVAS_PADDING;
	canvas.width = cairo.width - 2 * CANVAS_PADDING;
	canvas.height = cairo.height - 2 * CANVAS_PADDING - ct_label_height;

	bg_draw(widget, cr, &canvas);

	cairo_set_line_width (cr, 1);
	cairo_set_dash(cr, dash, 0, 0);
	/* TODO : try to draw all graph using gthread */
	for (i = 0; i < ncore; i++) {
		int j;
		int h = ct_core[i].ct->head;
		int t = ct_core[i].ct->tail;
		int s = ct_core[i].ct->size - 1;
		int dt = SLICE_SECOND MS + SLICE_MSECOND;
		int *data = ct_core[i].ct->data;
		int temp_first = data[h];
		int time_first = (t == 0 || t > h) ? 60 MS : (h - t) * dt;
		int x = TIME_TO_CAIRO(time_first, time_max, canvas.width);
		int y = TEMP_TO_CAIRO(temp_first, temp_max, canvas.height);

		if (h == t) continue;

		cairo_set_source_rgb(cr, rgb[i].r, rgb[i].g, rgb[i].b);
		ct_label_draw(cr, &canvas, i);
		cairo_move_to(cr, x, y);

		for (j = h - 1; j >= ((t > h) ? 0 : t); j--) {
			int time = time_first - (h - j) * dt;
			ct_draw(cr, &canvas, time, data[j]);
		}
		for (j = s; j > h; j--) {
			int time = time_first - ((s - j + h) * dt);
			ct_draw(cr, &canvas, time, data[j]);
		}
		cairo_stroke(cr);
	}
#ifdef GTK_3
#else
	cairo_destroy(cr);
#endif

	return TRUE;
}
