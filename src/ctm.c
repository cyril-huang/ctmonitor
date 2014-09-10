#include <stdlib.h>
#include <glib/gi18n.h>
#include "ctm.h"
#include "draw_graph.h"

struct ct_info *ct_core;
struct system_ct_info system_ct;
int ncore;

void repaint(GtkWidget *w)
{
	gtk_widget_queue_draw(w);
}

#ifdef GTK_3
#define DRAW_EVENT	"draw"
#else
#define DRAW_EVENT	"expose_event"
#endif

int main (int argc, char *argv[])
{
	GtkBuilder *builder;
	GObject *window;
	GObject *frame;
	GObject *save, *preference, *quit, *help, *about;
	GObject *drawarea;
	guint ret;

	gtk_init(&argc, &argv);

	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	init_ctm();
	init_ctm_draw();

	builder = gtk_builder_new();
	ret = gtk_builder_add_from_file(builder, UIMGRDIR"/ctm.ui", NULL);
	if (ret == 0) {
		fprintf(stderr, UIMGRDIR"/ctm.ui builder error\n");
		exit(1);
	}

	window = gtk_builder_get_object(builder, "window");
	frame = gtk_builder_get_object(builder, "frame");
	save = gtk_builder_get_object(builder, "Save");
	preference = gtk_builder_get_object(builder, "Preference");
	quit = gtk_builder_get_object(builder, "Quit");
	about = gtk_builder_get_object(builder, "About");
	drawarea = gtk_builder_get_object(builder, "drawarea");

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(about, "activate", G_CALLBACK(ctm_about), NULL);
	g_signal_connect(drawarea, DRAW_EVENT, G_CALLBACK(draw_graph), NULL);

	gtk_frame_set_label(GTK_FRAME(frame), _("CPU Temperature"));
	gtk_widget_set_size_request(GTK_WIDGET(window), WIN_WIDTH, WIN_HEIGHT);

	g_timeout_add(DRAW_SPEED, (GSourceFunc) repaint, GTK_WIDGET(drawarea));
	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
}
