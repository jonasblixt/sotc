bin_PROGRAMS = tcm

tcm_SOURCES = main.c \
			  render.c \
			  nanovg/nanovg.c

if OSX
tcm_SOURCES += nanovg/nanovg_mtl.m
endif

tcm_CFLAGS = -I$(top_srcdir)/include \
			  -I$(top_srcdir)/src/nanovg \
			  $(JSONC_CFLAGS) \
			  $(SDL_CFLAGS)

tcm_LDFLAGS =

if OSX
tcm_LDFLAGS += -framework Metal
tcm_LDFLAGS += -framework QuartzCore 
tcm_LDFLAGS += -framework CoreFoundation
endif

tcm_LDADD = $(top_builddir)/lib/libtcm.la \
			$(JSONC_LIBS) \
			$(SDL_LIBS)

