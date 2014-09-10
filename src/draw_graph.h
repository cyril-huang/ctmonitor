#ifndef _DRAW_GRAPH_H
#define _DRAW_GRAPH_H

typedef struct coordinate {
	int x0;			/* the origin x related to cairo system */
	int y0;			/* the origin y related to cairo system */
	int width;		/* the max width of coordinate */
	int height;		/* the max height of coordinate */
} coordinate_t;

#define WIN_WIDTH	500	/* the default window width */
#define WIN_HEIGHT	500	/* the default window height */

#define CANVAS_PADDING	30	/* the padding to the cairo system */
#define ROW_HEIGHT_CT	40	/* the row height for the coretemp info */

#define GRID_TIME_MIN_INTERVAL 50
#define GRID_TEMP_MIN_INTERVAL 20
#define GRID_TIME_NUM_INTERVAL 6

#define CT_LABEL_HEIGHT_PER_ROW 50
#define CT_LABEL_NUM_PER_ROW  4	/* the number of coretemp label per row */
#define CT_LABEL_BRICK_WIDTH  30/* the width of the brick */
#define CT_LABEL_BRICK_HEIGHT 20/* the height of the brick */

#define DRAW_SPEED	120	/* draw the graph every 120 ms */

typedef struct rgb {
	double r;
	double g;
	double b;
} rgb_t;

void init_ctm_draw();
#ifdef GTK_3
gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data);
#else
gboolean draw_graph(GtkWidget *widget, GdkEventExpose *event, gpointer data);
#endif
#endif /* _DRAW_GRAPH_H */
