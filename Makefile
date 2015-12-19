#makefile for BUGS I - SRN 7/12/91 
# modified for Linux on 6/15/92 Ken Corey

PROG = bugs

CC       ?= gcc
CFLAGS   ?= -O3
CFLAGS	 += -std=c99 -Wall -Wextra
CFLAGS   += $(shell ncurses5-config --cflags)
CPPFLAGS += -DLINUX
CPPFLAGS += -D_POSIX_C_SOURCE=199309L
CPPFLAGS += -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED
LIBS     += $(shell ncurses5-config --libs)

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = info.h

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)

bugs.o: bugs.h

clean: 
	$(RM) $(PROG) $(OBJS)
.PHONY: clean
