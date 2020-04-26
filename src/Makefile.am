bin_PROGRAMS = tcm

tcm_SOURCES = main.c \
					   nanovg/nanovg.c

tcm_CFLAGS = -I$(top_srcdir)/include \
					  -I$(top_srcdir)/src/nanovg \
					  $(JSONC_CFLAGS) \
					  $(GLFW_CFLAGS)

tcm_LDFLAGS =

if OSX
tcm_CFLAGS += -DGL_SILENCE_DEPRECATION
tcm_LDFLAGS += -framework OpenGL -framework GLUT
endif

tcm_LDADD = $(JSONC_LIBS) \
					$(GLFW_LIBS)

