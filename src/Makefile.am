bin_PROGRAMS = sotc

sotc_SOURCES = main.c


sotc_CFLAGS = -I$(top_srcdir)/include \
			  $(JSONC_CFLAGS)

sotc_LDFLAGS =

sotc_LDADD = $(top_builddir)/lib/libsotc.la \
			$(JSONC_LIBS)

