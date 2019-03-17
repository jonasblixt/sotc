# Stateblaster makefile


APPLICATION = stateblaster

CC ?= clang

CFLAGS  = -std=c11 -O2 -Wall -pedantic
CFLAGS += -I 3pp/nanovg

#ifeq ($(OS), OSX)
CFLAGS += -DGL_SILENCE_DEPRECATION
CFLAGS += -lglfw -framework OpenGL -framework GLUT
CFLAGS += -I include/ -I 3pp/ufsm/src
CFLAGS += -I include/sb

#endif

C_SRCS  = main.c
C_SRCS += gui/state.c
C_SRCS += gui/grid.c
C_SRCS += gui/menu.c
C_SRCS += 3pp/nanovg/nanovg.c
C_SRCS += 3pp/ufsm/src/ufsm.c
C_SRCS += 3pp/ufsm/src/ufsm_stack.c
C_SRCS += 3pp/ufsm/src/ufsm_queue.c

OBJS = $(C_SRCS:.c=.o)

all: $(APPLICATION)

$(APPLICATION): $(OBJS)
	$(info Link $(APPLICATION))
	@$(CC) $(CFLAGS) $(OBJS) -o $(APPLICATION)

clean:
	@-rm -f $(APPLICATION)
	@-rm -f $(OBJS)



