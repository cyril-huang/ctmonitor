#include <glib/gi18n.h>
#include "ctm.h"

void ctm_about(GtkAction *action, gpointer data)
{
	const gchar * const authors[] = {
		_("Gyoza Huang"),
		_("Shwan Chiou"),
		NULL
	};

	const gchar * const documenters[] = {
		_("Gyoza Huang"),
		_("Cisco Systems"),
		NULL
	};

	const gchar * const artists[] = {
		_("Gyoza Huang"),
		NULL
	};

	gtk_show_about_dialog (
			NULL,
			"name",                 _("Coretemp Monitor"),
			"comments",             _("CPU core temperature monitor"),
			"version",              VERSION,
			"copyright",            "Copyright \xc2\xa9 2011 Gyoza Huang\n",
			"logo-icon-name",       "utilities-system-monitor",
			"authors",              authors,
			"artists",              artists,
			"documenters",          documenters,
			"translator-credits",   _("translator-credits"),
			"license",              "GPL 2+",
			"wrap-license",         TRUE,
			NULL
			);
}
