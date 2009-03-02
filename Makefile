###########################################################################
# $Id: Makefile 1103 2009-01-12 21:19:36Z zalila $
###########################################################################

# FICHIERS SOURCES DE L'UTILISATEUR (UN FICHIER DONNERA LIEU À UN
# EXECUTABLE). LES VARIABLES CORRESPONDANT A UN LANGAGE DE
# PROGRAMMATION NON UTILISE (C ou ADA) PEUVENT ETRE VIDES

FICHIERS_UTILISATEUR_C = simple_test.c demo.c
FICHIERS_UTILISATEUR_ADA = simple_test_ada.adb

###########################################################################
# NE RIEN MODIFIER APRES CETTE LIGNE
###########################################################################

FICHIERS_UTILISATEUR := $(FICHIERS_UTILISATEUR_C) $(FICHIERS_UTILISATEUR_ADA)

EXECUTABLES_C = $(FICHIERS_UTILISATEUR_C:.c=)
EXECUTABLES_ADA = $(FICHIERS_UTILISATEUR_ADA:.adb=)
EXECUTABLES = $(EXECUTABLES_C) $(EXECUTABLES_ADA)

ALIS_UTILISATEUR = $(FICHIERS_UTILISATEUR_ADA:.adb=.ali)

CFLAGS  := -g -Wall -Werror
LDFLAGS := -L/usr/X11/lib -lX11 -lpthread

CUSTOM_LDFLAGS = -L. -lsimplified_graphics

all: $(EXECUTABLES)

$(EXECUTABLES_C): $(FICHIERS_UTILISATEUR_C)
	gcc $(CFLAGS) -c $@.c
	gcc -o $@ $@.o $(CUSTOM_LDFLAGS) $(LDFLAGS)

$(EXECUTABLES_ADA): $(FICHIERS_UTILISATEUR_ADA)
	gnat compile $@.adb -cargs $(CFLAGS)
	gnat bind -x $@.ali
	gnat link $@.ali $(CUSTOM_LDFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(ALIS_UTILISATEUR) *~ $(EXECUTABLES)
	rm -rf *.dSYM 
