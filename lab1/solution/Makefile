CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude
SRCDIR  := src

PARENT_OBJS := main.c         $(SRCDIR)/common.c
CHILD1_OBJS := $(SRCDIR)/child1.c $(SRCDIR)/common.c $(SRCDIR)/transform.c
CHILD2_OBJS := $(SRCDIR)/child2.c $(SRCDIR)/common.c $(SRCDIR)/transform.c

BINS := parent child1 child2

all: $(BINS)

parent: $(PARENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

child1: $(CHILD1_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

child2: $(CHILD2_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./parent

clean:
	rm -f $(BINS)

.PHONY: all run clean