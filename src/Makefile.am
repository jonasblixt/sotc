bin_PROGRAMS = sotc

sotc_SOURCES = main.c \
			   object_tree.c \
			   canvas/view.c \
			   canvas/state.c \
			   canvas/region.c \
			   canvas/grid.c \
			   canvas/selection.c \
			   canvas/controller.c


sotc_CFLAGS = -I$(top_srcdir)/include \
			  -DGTK_DISABLE_DEPRECATED \
			  -DGLIB_DISABLE_DEPRECATED \
			  $(JSONC_CFLAGS) \
			  $(GTK_CFLAGS) \
			  $(CAIRO_CFLAGS) \
			  $(GOOCANVAS_CFLASG) \
			  $(PANGO_CFLASG)

sotc_LDFLAGS =

sotc_LDADD = $(top_builddir)/lib/libsotc.la \
			$(JSONC_LIBS) \
			$(GTK_LIBS) \
			$(CAIRO_LIBS) \
			$(GOOCANVAS_LIBS) \
			$(PANGO_LIBS)

