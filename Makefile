###########################################################################
# $Id: Makefile 1103 2009-01-12 21:19:36Z zalila $
###########################################################################

FICHIERS_UTILISATEUR = simple_test.c demo.c

EXECUTABLES = $(FICHIERS_UTILISATEUR:.c=)

CFLAGS  := -g -Wall -Werror
LDFLAGS := -L/usr/X11/lib -lX11 -lpthread

HEADERS = graphiqu_enis.h
SOURCES = graphiqu_enis.c
OBJECTS = $(SOURCES:.c=.o)

LIBRARY = libsimplified_graphics.a
CUSTOM_LDFLAGS = -L. -lsimplified_graphics

all: $(EXECUTABLES)

$(EXECUTABLES): $(FICHIERS_UTILISATEUR) $(LIBRARY)
	gcc $(CFLAGS) -c $@.c
	gcc -o $@ $@.o $(CUSTOM_LDFLAGS) $(LDFLAGS)

dist: allclean
	rm -rf libsimplified_graphics
	mkdir libsimplified_graphics
	cp Makefile $(FICHIERS_UTILISATEUR) $(SOURCES) $(HEADERS) \
	  libsimplified_graphics
	$(MAKE) -f ../mk.dev -C libsimplified_graphics $(LIBRARY)
	$(MAKE) -C libsimplified_graphics clean
	tar czvf libsimplified_graphics.tar.gz libsimplified_graphics

clean:
	rm -f *.o *~ $(EXECUTABLES)
	rm -rf *.dSYM 

allclean: clean
	rm -f $(LIBRARY)
	rm -rf libsimplified_graphics