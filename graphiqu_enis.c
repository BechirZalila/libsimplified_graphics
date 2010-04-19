/* $Id$ */

/*

  Copyright (C) 2009 Bechir Zalila <bechir.zalila@aist.enst.fr>
  Voir le fichier COPYING pour la licence

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <pthread.h>

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#define CX(x) (w_width * ((x) - _xmin) / (_xmax - _xmin))
#define CY(y) (w_height - w_height * ((y) - _ymin) / (_ymax - _ymin))

char graphiqu_enis_version_string [] = "1.1";

int initialized = 0;

Display *d;  /* Le display */
int s;       /* L'écran */
Window w;    /* La fenêtre */
GC gc;       /* Le contexte graphique */
Pixmap bkp;  /* Une sauvegarde de ce qu'il y a sur la fenêtre */

unsigned int requested_width = 0, requested_height = 0;
/* Dimensions demandées par l'utilisateur. On peut par exemple avoir
   des coordonnées qui s'étendent par exemple entre -1 et 1 alors que
   la taille de la fenêtre est de 100 pixels */

unsigned int w_width, w_height;   /* Dimensions de la fenêtre */
float _xmin, _ymin, _xmax, _ymax; /* Sauvegardes des coordonnées */

float _cur_x, _cur_y; /* Position courante (utile pour l'écriture de
			 texte). */

XFontStruct* font_info = NULL; /* Police courante */

/* Nom générique de police redimensionnable. Le '%d' sera remplacé
   pour correspondre à la taille choisie par l'utilisateur. */
char* font_name_template 
  = "-*-*-medium-r-normal--0-%d-75-75-p-0-iso8859-1";

int hauteur_texte = 10; /* Hauteur par défaut du texte */

void sync_font_info (void); 
/* Garantit la cohérence entre hauteur_texte et font_info */

Colormap colmap;  /* Table des couleurs système */

XColor TableCoul [256];
/* Table des couleurs. Les 16 premières valeurs seront remplies par
   les couleurs usuelles. Les 240 couleurs restantes forment un
   dégradé du vert au bleu-verdatre en passant par le jaune, le rouge,
   le violet et le bleu. Toutes les couleurs peuvent être modifiées
   par l'utilisateur. */

void init_couleurs (void);
/* Initialise les 256 couleurs */

pthread_t main_l;
/* Thread exécutant la boucle d'évènements */

void main_loop (void);
/* Boucle principale de la fenêtre graphique (sera exécutée dans un
   thread indépendant). */

void Graphiqu_ENIS_Version (void) {
  printf ("Bibliothèque graphique simplifiée de l'ENIS. Version %s\n",
	  graphiqu_enis_version_string);
  printf ("Copyright 2009 Bechir Zalila (bechir.zalila@aist.enst.fr)\n");
}

void Dimensions_Fenetre (int largeur, int hauteur) {
  requested_width = largeur;
  requested_height = hauteur;
}

void Initialisation_Graphique (float xmin, 
			       float ymin, 
			       float xmax, 
			       float ymax)
{
  XEvent e;
  unsigned int line_width = 1;
  int line_style = LineSolid;
  int cap_style = CapRound;
  int join_style = JoinRound;

  /* Vérifier que les dimensions données sont cohérentes */

  assert ((xmax - xmin) > 0);
  assert ((ymax - ymin) > 0);

  _xmin = xmin;
  _ymin = ymin;
  _xmax = xmax;
  _ymax = ymax;

  if (requested_width == 0) {
    w_width = (unsigned int) (xmax - xmin);
  } else {
    w_width = requested_width;
  }

  if (requested_height == 0) {
    w_height = (unsigned int) (ymax - ymin);
  } else {
    w_height = requested_height;
  }

  _cur_x = _xmin;
  _cur_y = _ymin;

  /* Message de version et de copyright */

  Graphiqu_ENIS_Version ();

  /* Initialisation de XLib avec le support multi-tâches */

  if (!XInitThreads ()) {
    printf ("Impossible d'initialiser XLib avec le support multi-tâches\n");
    exit (1);
  }

  /* Ouvrir la connexion avec le serveur graphique */
 
  d = XOpenDisplay (NULL);
  if (d == NULL) {
    printf ("Impossible d'ouvrir l'affichage\n");
    exit (1);
  }
  
  /* Verrouillage du display */

  XLockDisplay (d);
  
  /* Récupération de l'écran par défaut */
  
  s = XDefaultScreen (d);

  /* Création de la fenêtre */

  w = XCreateSimpleWindow (d, 
			   RootWindow (d, s), 
			   10, 
			   10, 
			   w_width + 2, 
			   w_height + 2,
			   1, 
			   BlackPixel (d, s), 
			   WhitePixel (d, s));  

  /* Choisir les événements qui nous intéressent */
  
  XSelectInput (d, w, StructureNotifyMask | ExposureMask | KeyPressMask);

  /* Création du contexte graphique */

  gc = XCreateGC (d, RootWindow (d, s), 0, NULL);
  XSetBackground (d, gc, WhitePixel (d, s));
  XSetForeground (d, gc, BlackPixel (d, s));

  /* Propriétés des lignes tracées */

  XSetLineAttributes (d, gc, 
		      line_width, line_style,
		      cap_style, join_style);

  /* Essayer de trouver une police de caractères */

  sync_font_info ();

  /* Création de la table de pixel qui sauvegarde la fenêtre et
     coloriage de son arrière plan en blanc. */
  
  bkp = XCreatePixmap (d,
		       RootWindow (d, s),
		       w_width + 2,
		       w_height + 2,
		       DefaultDepth (d, DefaultScreen (d)));
  XSetForeground (d, gc, WhitePixel (d, s));
  XFillRectangle (d, bkp, gc, 0, 0, w_width + 2, w_height + 2);
  XSetForeground (d, gc, BlackPixel (d, s));

  /* Initialisation des couleurs */

  init_couleurs ();

  /* Afficher la fenêtre */
  
  XMapWindow (d, w);
  XFlush (d);

  /* Se bloquer jusqu'à l'apparition de la fenêtre */

  while (1) {
    XNextEvent (d, &e);
    if (e.type == MapNotify) {
      break;
    }
  }

  /* Déverrouillage du disaply */

  XUnlockDisplay (d);

  initialized = 1;

  /* Lancement de l'event dispatch thread de la fenêtre. */

  pthread_create (&main_l, NULL, (void *) main_loop, NULL);
}

void Fin_Graphique (void)
{
  assert (initialized);

  XLockDisplay (d);
  XFreePixmap (d, bkp);
  XFreeGC (d, gc);
  XDestroyWindow (d, w);
  XFlush (d);
  XCloseDisplay (d);
  XUnlockDisplay (d);

  initialized = 0;
}

void Efface (void)
{
  assert (initialized);

  /* Traçage d'un rectangle blanc */

  XLockDisplay (d);
  XSetForeground (d, gc, WhitePixel (d, s));
  XFillRectangle (d, w, gc, 0, 0, w_width, w_height);
  XFillRectangle (d, bkp, gc, 0, 0, w_width, w_height);
  XFlush (d);
  XUnlockDisplay (d);
}

void Trait (float x1, 
	    float y1, 
	    float x2, 
	    float y2, 
	    int couleur)
{
  float _x1 = CX (x1);
  float _y1 = CY (y1);
  float _x2 = CX (x2);
  float _y2 = CY (y2);

  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
  XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  XFlush (d);
  XUnlockDisplay (d);

}

void Croix (float x, 
	    float y, 
	    int couleur)
{
  float _x = CX (x);
  float _y = CY (y);
  int dim = 3;

  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawLine (d, w, gc, _x - dim, _y - dim, _x + dim, _y + dim);
  XDrawLine (d, bkp, gc, _x - dim, _y - dim, _x + dim, _y + dim);
  XDrawLine (d, w, gc, _x - dim, _y + dim, _x + dim, _y - dim);
  XDrawLine (d, bkp, gc, _x - dim, _y + dim, _x + dim, _y - dim);
  XFlush (d);
  XUnlockDisplay (d);
}

void Point (float x, 
	    float y, 
	    int couleur)
{
  float _x = CX (x);
  float _y = CY (y);

  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawPoint (d, w, gc, _x, _y);
  XDrawPoint (d, bkp, gc, _x, _y);
  XFlush (d);
  XUnlockDisplay (d);
}

void Cercle (float x, 
	     float y, 
	     int rayon, 
	     int couleur)
{
  float _x = CX (x);
  float _y = CY (y);

  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawArc (d, w, gc,
	    _x - rayon, _y - rayon,  /* Point de départ */ 
	    2 * rayon, 2 * rayon,    /* Point d'arrivée */
	    0, 360*64);              /* Angles (en 64e de degré) */
  XDrawArc (d, bkp, gc,
	    _x - rayon, _y - rayon,  /* Point de départ */ 
	    2 * rayon, 2 * rayon,    /* Point d'arrivée */
	    0, 360*64);              /* Angles (en 64e de degré) */
  XFlush (d);
  XUnlockDisplay (d);
}

void Disque (float x, 
	     float y, 
	     int rayon, 
	     int couleur)
{
  float _x = CX (x);
  float _y = CY (y);

  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillArc (d, w, gc,
	    _x - rayon, _y - rayon,  /* Point de départ */ 
	    2 * rayon, 2 * rayon,    /* Point d'arrivée */
	    0, 360*64);              /* Angles (en 64e de degré) */
  XFillArc (d, bkp, gc,
	    _x - rayon, _y - rayon,  /* Point de départ */ 
	    2 * rayon, 2 * rayon,    /* Point d'arrivée */
	    0, 360*64);              /* Angles (en 64e de degré) */
  XFlush (d);
  XUnlockDisplay (d);
}

void DrawPoly (int nbr_points, 
	       float *poly_points, 
	       int couleur)
{
  int i;
  float _x1, _x2, _y1, _y2;
  
  assert (initialized);
  assert (nbr_points > 1);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);

  for (i = 0; i < nbr_points - 1; i++) {
    /* Tracer une ligne entre le point i et le point i + 1 */

    _x1 = CX (poly_points [2 * i]);
    _y1 = CY (poly_points [2 * i + 1]);
    _x2 = CX (poly_points [2 * i + 2]);
    _y2 = CY (poly_points [2 * i + 3]);
    XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
    XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  }

  /* Relier le dernier point au premier point */
  
  _x1 = CX (poly_points [2 * (nbr_points - 1)]);
  _y1 = CY (poly_points [2 * nbr_points - 1]);
  _x2 = CX (poly_points [0]);
  _y2 = CY (poly_points [1]);
  XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
  XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  
  XFlush (d);  
  XUnlockDisplay (d);
}

void FillPoly (int nbr_points, 
	       float *poly_points, 
	       int couleur)
{
  XPoint *pol;
  int i;

  assert (initialized);
  assert (nbr_points > 1);

  pol = (XPoint *) malloc (nbr_points * sizeof (XPoint));

  for (i = 0; i < nbr_points; i++) {
    pol [i].x = CX (poly_points [2 * i]);
    pol [i].y = CY (poly_points [2 * i + 1]);
  }

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillPolygon (d, w, gc, pol, nbr_points, Complex, CoordModeOrigin);
  XFillPolygon (d, bkp, gc, pol, nbr_points, Complex, CoordModeOrigin);
  XFlush (d);
  XUnlockDisplay (d);

  free (pol);
}

void DrawTriangle (float *poly_points, 
		   int couleur)
{
  DrawPoly (3, poly_points, couleur);
}

void FillTriangle (float *poly_points, 
		   int couleur)
{
  FillPoly (3, poly_points, couleur);
}

void DrawRectangle (float x1, 
		    float y1, 
		    float x2, 
		    float y2, 
		    int couleur)
{
  float _x1 = CX (x1),
    _x2 = CX (x2),
    _y1 = CY (y1),
    _y2 = CY (y2);
  
  float _x_M = MAX (_x1, _x2),
    _x_m = MIN (_x1, _x2),
    _y_M = MAX (_y1, _y2),
    _y_m = MIN (_y1, _y2);
    
  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawRectangle (d, w, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));
  XDrawRectangle (d, bkp, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));  
  XFlush (d);
  XUnlockDisplay (d);
}

void FillRectangle (float x1, 
		    float y1, 
		    float x2, 
		    float y2, 
		    int couleur)
{
  float _x1 = CX (x1),
    _x2 = CX (x2),
    _y1 = CY (y1),
    _y2 = CY (y2);
  
  float _x_M = MAX (_x1, _x2),
    _x_m = MIN (_x1, _x2),
    _y_M = MAX (_y1, _y2),
    _y_m = MIN (_y1, _y2);
    
  assert (initialized);
  
  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillRectangle (d, w, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));
  XFillRectangle (d, bkp, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));  
  XFlush (d);
  XUnlockDisplay (d);
}

void CreateColorRGB (int numero,
		     int R,
		     int G,
		     int B)
{
  assert ((numero >= 0) && (numero <= 255));
  
  TableCoul [numero].red   = R * 256;
  TableCoul [numero].green = G * 256;
  TableCoul [numero].blue  = B * 256;
  TableCoul [numero].flags = DoRed | DoGreen | DoBlue;
    
  XLockDisplay (d);
  XAllocColor (d, colmap, &TableCoul [numero]);
  XUnlockDisplay (d);
}

void Hauteur_Texte (float hauteur)
{
  assert (initialized);
  
  hauteur_texte = (int) hauteur;
  
  sync_font_info ();
}

void itos (int n, 
	   char s[])
{
  assert (initialized);
  sprintf (s, "%d", n);
}

void Deplace (float x1,
	      float y1)
{
  assert (initialized);
  
  _cur_x = x1;
  _cur_y = y1;
}

void EcritXY (float x, 
	      float y, 
	      char *chaine,
	      int couleur)
{
  int _x = CX (x),
    _y = CY (y);
  
  assert (initialized);

  XLockDisplay (d);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawString (d, w, gc, _x, _y, chaine, strlen (chaine));
  XDrawString (d, bkp, gc, _x, _y, chaine, strlen (chaine));
  XFlush (d);
  XUnlockDisplay (d);
}

void Ecrit (char *chaine,
	    int couleur)
{
  EcritXY (_cur_x, _cur_y, chaine, couleur);
}

void Attente (void)
{
  char c;

  assert (initialized);

  while ((c = fgetc (stdin)) != '\n' && c != EOF);

  c = '\0';
  while (c != '\n') {
    scanf ("%c", &c);
  }
}

void main_loop (void) {
  XEvent e;
  int evenement_present;

  while (initialized) {
    XLockDisplay (d);

    /* Voir s'il y a des évènements en attente */
    evenement_present = XPending (d);

    if (evenement_present) {
      /* S'il y a des evènements, on les récupère */

      XNextEvent (d, &e);
      XUnlockDisplay (d);
    } else {
      /* S'il n'y a pas de nouveaux évènement, on fait dormir le
	 thread pendant une brève durée (pour éviter d'occuper 100% du
	 CPU) et on continue la boucle. On estime que 0.1 seconde est
	 suffisante pour garantir un affichage agréable de la
	 fenêtre.
      */
      
      XUnlockDisplay (d);
      usleep (100000);
      continue;
    }

    if (e.type == Expose) {
      /* Si une partie de la fenêtre doit être redessinée, on la copie
	 depuis la table de pixel de sauvegarde. */

      XLockDisplay (d);
      XCopyArea (d, bkp, w, gc,
		 e.xexpose.x, e.xexpose.y,
		 e.xexpose.width, e.xexpose.height,
		 e.xexpose.x, e.xexpose.y);
      XUnlockDisplay (d);
    } 
  }
}

void init_couleurs (void) {
  char couleursPredef [16][15] = 
    {"black", 
     "blue", 
     "green", 
     "cyan", 
     "red",
     "darkmagenta", 
     "brown", 
     "lightgray", 
     "darkgray",
     "lightblue", 
     "lightgreen", 
     "lightcyan", 
     "orange", 
     "magenta", 
     "yellow", 
     "white"};
  
  int i;
  XColor exact;

  /* Pas de couleurs. On divise l'intervalle [0, 255] en 41
     sous-intervalles pour pouvoir avoir un dégradé réaliste sur 240
     couleurs. */
  int steps [42] = {0, 6, 12, 18, 25, 31, 37, 43, 50, 56, 62, 68, 75, 81, 
		    87, 93, 100, 106, 112, 118, 125, 131, 137, 143, 150, 156,
		    162, 168, 175, 181, 187, 193, 200, 206, 212, 218, 225, 
		    231, 237, 243, 249, 255};

  /* États de la machine à états qui génère le dégradé */
  enum p_states {i_green,         /* Augmente G à R et B nuls */
		 s_green_i_red,   /* Augmente R à G maximum et B nul */
		 s_red_d_green,   /* Diminue G à R maximum et B nul */
		 s_red_i_blue,    /* Augmente B à R maximum et G nul */
		 s_blue_d_red,    /* Diminue R à B maximum et G nul */
		 s_blue_i_green}; /* Augmente G à B maximum et R nul */

  enum p_states cur_state = i_green;
  int count, step;
  
  XLockDisplay (d);
  colmap = DefaultColormap (d, s);
  
  /* Construction des 16 premières couleurs */

  for (i = 0; i < 16; i++) {
    XAllocNamedColor (d, colmap, couleursPredef [i], &TableCoul [i], &exact);
  }
  XUnlockDisplay (d);

  /* Construction du dégradé sur les 240 positions restantes */

  count = 16;
  step = 12;

  while (count < 256) {
    switch (cur_state) 
      {
      case i_green :
	CreateColorRGB (count, steps [0], steps [step], steps [0]);
	if (step == 41) {
	  cur_state = s_green_i_red;
	  step = 0;
	} else {
	  step++;
	}
	break;

      case s_green_i_red :
	CreateColorRGB (count, steps [step], steps [41], steps [0]);
	if (step == 41) {
	  cur_state = s_red_d_green;
	  step = 41;
	} else {
	  step++;
	}
	break;

      case s_red_d_green :
	CreateColorRGB (count, steps [41], steps [step], steps [0]);
	if (step == 0) {
	  cur_state = s_red_i_blue;
	  step = 0;
	} else {
	  step--;
	}
	break;

      case s_red_i_blue :
	CreateColorRGB (count, steps [41], steps [0], steps [step]);
	if (step == 41) {
	  cur_state = s_blue_d_red;
	  step = 41;
	} else {
	  step++;
	}
	break;

      case s_blue_d_red :
	CreateColorRGB (count, steps [step], steps [0], steps [41]);
	if (step == 0) {
	  cur_state = s_blue_i_green;
	  step = 0;
	} else {
	  step--;
	}
	break;

      case s_blue_i_green :
	CreateColorRGB (count, steps [0], steps [step], steps [41]);
	if (step == 41) {
	  cur_state = i_green;
	  step = 0;
	  break;
	} else {
	  step++;
	}
	break;
      }
    count++;
  }
}

void sync_font_info (void) {
  char the_font_name [100];

  sprintf (the_font_name, font_name_template, hauteur_texte * 10);

  XLockDisplay (d);

  if (font_info != NULL) {
    XFreeFont (d, font_info);
  }

  font_info = XLoadQueryFont (d, the_font_name);

  if (font_info == NULL) {
    XUnlockDisplay (d);
    fprintf (stderr, "XLoadQueryFont: échec du chargement de"
	    " la police de caractère '%s'\n", the_font_name);
    exit (1);
  }
  else {
    XSetFont (d, gc, font_info->fid);
    XUnlockDisplay (d);
  }
}
