###########################################################################
# $Id: Makefile 1103 2009-01-12 21:19:36Z zalila $
###########################################################################

# FICHIERS SOURCES DE L'UTILISATEUR (UN FICHIER DONNERA LIEU À UN EXECUTABLE)

FICHIERS_UTILISATEUR = simple_test.c demo.c

###########################################################################
# NE RIEN MODIFIER APRES CETTE LIGNE
###########################################################################

EXECUTABLES = $(FICHIERS_UTILISATEUR:.c=)

CFLAGS  := -g -Wall -Werror
LDFLAGS := -L/usr/X11/lib -lX11 -lpthread

CUSTOM_LDFLAGS = -L. -lsimplified_graphics

all: $(EXECUTABLES)

$(EXECUTABLES): $(FICHIERS_UTILISATEUR)
	gcc $(CFLAGS) -c $@.c
	gcc -o $@ $@.o $(CUSTOM_LDFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(EXECUTABLES)
	rm -rf *.dSYM 
