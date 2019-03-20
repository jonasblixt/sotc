# Stateblaster makefile


APPLICATION = stateblaster

CC ?= clang

CFLAGS  = -std=c11 -O2 -Wall -pedantic
CFLAGS += -I 3pp/nanovg

#ifeq ($(OS), OSX)
CFLAGS += -DGL_SILENCE_DEPRECATION
CFLAGS += -I include/ -I 3pp/ufsm/src
CFLAGS += -I include/sb

LDFLAGS = -lglfw -framework OpenGL -framework GLUT
#endif

C_SRCS  = main.c
C_SRCS += gui/component.c
C_SRCS += gui/state.c
C_SRCS += gui/grid.c
C_SRCS += gui/menu.c
C_SRCS += 3pp/nanovg/nanovg.c
C_SRCS += 3pp/ufsm/src/ufsm.c
C_SRCS += 3pp/ufsm/src/ufsm_stack.c
C_SRCS += 3pp/ufsm/src/ufsm_queue.c
C_SRCS += sb_machine.c

XMI_SRCS = sb.xmi


OBJS = $(C_SRCS:.c=.o)
XMI_OBJS = $(XMI_SRCS:.xmi=.xmio)

all: $(APPLICATION)

%.o : %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.xmio : %.xmi
	@echo UFSMIMPORT $<
	@ufsmimport $< "sb_machine"

$(APPLICATION): $(XMI_OBJS) $(OBJS) 
	$(info Link $(APPLICATION))
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(APPLICATION)

clean:
	@-rm -f $(APPLICATION)
	@-rm -f $(OBJS)



