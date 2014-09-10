uimanagerdir = @datadir@/@PACKAGE@/ui
uimanager_DATA = ctm.ui

AM_CFLAGS = -DUIMGRDIR=\"$(uimanagerdir)\" \
	    -DLOCALEDIR=\"@datadir@/locale\" \
	    @GTK_CFLAGS@
if DEBUG
AM_CFLAGS += -DDEBUG
endif

if with_gtk3
AM_CFLAGS += -DGTK_3
endif

bin_PROGRAMS = ctmonitor
ctmonitor_SOURCES = ctm.h draw_graph.h ctm.c ctm_util.c readct.c draw_graph.c interface.c
ctmonitor_LDADD = @GTK_LIBS@
