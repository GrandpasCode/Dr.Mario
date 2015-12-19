#makefile for BUGS I - SRN 7/12/91 
# modified for Linux on 6/15/92 Ken Corey

PROG = bugs

CC       ?= gcc
CFLAGS   ?= -O3
CFLAGS   += $(shell ncurses5-config --cflags)
CPPFLAGS += -DLINUX
LIBS     += $(shell ncurses5-config --libs)

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = info.h

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)

bugs.o: bugs.h

clean: 
	$(RM) $(PROG) $(OBJS)
.PHONY: clean
