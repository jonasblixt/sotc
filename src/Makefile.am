bin_PROGRAMS = sotc

sotc_SOURCES = main.c \
			   state_canvas.c \
			   object_tree.c


sotc_CFLAGS = -I$(top_srcdir)/include \
			  -DGTK_DISABLE_DEPRECATED \
			  -DGLIB_DISABLE_DEPRECATED \
			  $(JSONC_CFLAGS) \
			  $(GTK_CFLAGS) \
			  $(CAIRO_CFLAGS)

sotc_LDFLAGS =

sotc_LDADD = $(top_builddir)/lib/libsotc.la \
			$(JSONC_LIBS) \
			$(GTK_LIBS) \
			$(CAIRO_LIBS)

