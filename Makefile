CC=gcc
CFLAGS=-o
TARGETS=smsh
all: $(TARGETS)
.PHONY: all
%:
	$(CC) -g -o $@ $@.c

clean:
	rm $(TARGETS)
