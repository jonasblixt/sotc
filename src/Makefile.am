bin_PROGRAMS = stateblaster

stateblaster_SOURCES = main.c \
					   nanovg/nanovg.c

stateblaster_CFLAGS = -I$(top_srcdir)/include \
					  -I$(top_srcdir)/src/nanovg \
					  $(JSONC_CFLAGS) \
					  $(GLFW_CFLAGS)

stateblaster_LDFLAGS =

if OSX
stateblaster_CFLAGS += -DGL_SILENCE_DEPRECATION
stateblaster_LDFLAGS += -framework OpenGL -framework GLUT
endif

stateblaster_LDADD = $(JSONC_LIBS) \
					$(GLFW_LIBS)

