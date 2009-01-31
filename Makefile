###########################################################################
# $Id: Makefile 1103 2009-01-12 21:19:36Z zalila $
###########################################################################

CFLAGS  := -g -Wall -Werror
LDFLAGS := -L/usr/X11/lib -lX11 -lpthread

HEADERS = graphiqu_enis.h
SOURCES = graphiqu_enis.c
OBJECTS = $(SOURCES:.c=.o)

LIBRARY = libsimplified_graphics.a

all: $(OBJECTS) simple_test demo

$(LIBRARY): $(OBJECTS)

$(OBJECTS): $(SOURCES)
	gcc $(CFLAGS) -c $<

simple_test: simple_test.c $(OBJECTS)
	gcc $(CFLAGS) -c simple_test.c
	gcc -o simple_test simple_test.o graphiqu_enis.o $(LDFLAGS)

demo: demo.c $(OBJECTS)
	gcc $(CFLAGS) -c demo.c
	gcc -o demo demo.o graphiqu_enis.o $(LDFLAGS)

clean:
	rm -f *.o *~ simple_test demo
	rm -rf *.dSYM 
