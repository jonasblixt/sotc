lib_LTLIBRARIES = libtcm.la

libtcm_la_SOURCES = tcm.c \
					stack.c \
					model.c \
					state.c

libtcm_la_CFLAGS = -I$(top_srcdir)/include \
                     $(CODE_COVERAGE_CFLAGS) \
					 $(JSONC_CFLAGS)

libtcm_la_LIBS = $(JSONC_LIBS) \
				 $(CODE_COVERAGE_LIBS)
