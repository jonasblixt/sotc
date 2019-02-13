# Stateblaster makefile


APPLICATION = stateblaster

CC ?= clang

CFLAGS  = -std=c11 -O2 -Wall -pedantic
CFLAGS += -I 3pp/nanovg

#ifeq ($(OS), OSX)
CFLAGS += -DGL_SILENCE_DEPRECATION
CFLAGS += -lglfw -framework OpenGL -framework GLUT
CFLAGS += -I include/
#endif

C_SRCS  = main.c
C_SRCS += demo.c
C_SRCS += 3pp/nanovg/nanovg.c

OBJS = $(C_SRCS:.c=.o)

all: $(APPLICATION)

$(APPLICATION): $(OBJS)
	$(info Link $(APPLICATION))
	@$(CC) $(CFLAGS) $(OBJS) -o $(APPLICATION)

clean:
	@-rm -f $(APPLICATION)
	@-rm -f $(OBJS)



