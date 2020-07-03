lib_LTLIBRARIES = libsotc.la

libsotc_la_SOURCES = sotc.c \
					stack.c \
					model.c \
					state.c \
					region.c

libsotc_la_CFLAGS = -I$(top_srcdir)/include \
                     $(CODE_COVERAGE_CFLAGS) \
					 $(JSONC_CFLAGS)

libsotc_la_LIBS = $(JSONC_LIBS) \
				 $(CODE_COVERAGE_LIBS)
