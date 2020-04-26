lib_LTLIBRARIES = libtcm.la

libtcm_la_SOURCES = tcm.c \
					stack.c \
					model.c

libtcm_la_CFLAGS = -I$(top_srcdir)/include \
							$(JSONC_CFLAGS)

libtcm_la_LIBS = $(JSONC_LIBS)
