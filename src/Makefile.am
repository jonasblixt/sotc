bin_PROGRAMS = tcm

tcm_SOURCES = main.c \
					   nanovg/nanovg.c

tcm_CFLAGS = -I$(top_srcdir)/include \
					  -I$(top_srcdir)/src/nanovg \
					  $(JSONC_CFLAGS) \
					  $(GLFW_CFLAGS)

tcm_LDFLAGS =

tcm_LDADD = $(JSONC_LIBS) \
			$(GLFW_LIBS)

if OSX
tcm_CFLAGS += -DGL_SILENCE_DEPRECATION
tcm_LDFLAGS += -framework OpenGL -framework GLUT
endif

if LINUX
tcm_CFLAGS += $(GL_CFLAGS) \
			  $(GLEW_CFLAGS) \
			  -DNANOVG_GLEW

tcm_LDADD += $(GL_LIBS) \
			 $(GLEW_LIBS)

tcm_LDFLAGS += -lm
endif


