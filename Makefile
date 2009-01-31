###########################################################################
# $Id: Makefile 1103 2009-01-12 21:19:36Z zalila $
###########################################################################

CFLAGS  := -g -Wall -Werror
LDFLAGS := -L/usr/X11/lib -lX11 -lpthread

HEADERS = graphiqu_enis.h
SOURCES = graphiqu_enis.c
OBJECTS = $(SOURCES:.c=.o)

LIBRARY = libsimplified_graphics.a
CUSTOM_LDFLAGS = -L. -lsimplified_graphics

all: $(OBJECTS) simple_test demo

$(LIBRARY): $(OBJECTS)

$(OBJECTS): $(SOURCES)
	gcc $(CFLAGS) -c $<

simple_test: simple_test.c $(LIBRARY)
	gcc $(CFLAGS) -c simple_test.c
	gcc -o simple_test simple_test.o $(CUSTOM_LDFLAGS) $(LDFLAGS)

demo: demo.c $(LIBRARY)
	gcc $(CFLAGS) -c demo.c
	gcc -o demo demo.o $(CUSTOM_LDFLAGS) $(LDFLAGS)

$(LIBRARY): $(OBJECTS)
	rm -f $@
	ar rcs $@ $(OBJECTS)
	ranlib $@

clean:
	rm -f *.o *~ simple_test demo
	rm -rf *.dSYM 
