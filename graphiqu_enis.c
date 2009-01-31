/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <assert.h>
#include <pthread.h>

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

int initialized = 0;

Display *d;  /* Le display */
int s;       /* L'écran */
Window w;    /* La fenêtre */
GC gc;
Pixmap bkp;  /* Une sauvegarde de ce qu'il y a sur la fenêtre */

unsigned int w_width, w_height;   /* Dimensions de la fenêtre */
float _xmin, _ymin, _xmax, _ymax; /* Sauvegardes des coordonnées */

float _cur_x, _cur_y;

XFontStruct* font_info = NULL;
char* font_name_template 
  = "-adobe-helvetica-medium-r-normal--0-%d-75-75-p-0-iso8859-1";
int hauteur_texte = 10;
void sync_font_info (void);

Colormap colmap;  /* Table des couleurs système */

XColor TableCoul [256];
/* Table des couleurs. Les 16 premières valeurs seront remplies par
   les couleurs usuelles. Le reste peut être alloué dynamiquement puis
   utilisé par l'utilisteur. */

void init_couleurs (void);

pthread_t main_l;

void main_loop (void);
/* Boucle principale de la fenêtre graphique (sera exécutée dans un
   thread indépendant). */

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

  w_width = (unsigned int) (xmax - xmin);
  w_height = (unsigned int) (ymax - ymin);

  _cur_x = _xmin;
  _cur_y = _ymin;

  /* Ouvrir la connexion avec le serveur graphique */
 
  d = XOpenDisplay (NULL);
  if (d == NULL) {
    printf ("Impossible d'ouvrir l'affichage\n");
    exit (1);
  }

  /* Récupération de l'écran par défaut */
  
  s = XDefaultScreen (d);

  /* Création de la fenêtre */

  w = XCreateSimpleWindow (d, 
			   RootWindow (d, s), 
			   10, 
			   10, 
			   w_width, 
			   w_height,
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

  XSetLineAttributes(d, gc, 
		     line_width, line_style,
		     cap_style, join_style);

  /* Essayer de trouver une police de caractères */

  sync_font_info ();

  /* Création de la table de pixel qui sauvegarde la fenêtre et
     coloriage de son arrière plan en blanc. */
  
  bkp = XCreatePixmap (d,
		       RootWindow (d, s),
		       w_width,
		       w_height,
		       DefaultDepth(d, DefaultScreen(d)));
  XSetForeground (d, gc, WhitePixel (d, s));
  XFillRectangle(d, bkp, gc, 0, 0, w_width, w_height);
  XSetForeground (d, gc, BlackPixel (d, s));

  /* Initialisation des 16 premières couleurs */

  init_couleurs ();

  /* Afficher la fenêtre */
  
  XMapWindow(d, w);
  XFlush (d);

  /* Se bloquer jusqu'à l'apparition de la fenêtre */

  while (1) {
    XNextEvent (d, &e);
    if (e.type == MapNotify) {
      break;
    }
  }

  initialized = 1;

  /* Lancement de l'event dispatch thread de la fenêtre. DESACTIVE
     pour le moment car les routines de X ne sont pas thread-safe. */
#ifndef __APPLE__
  pthread_create (&main_l, NULL, (void *) main_loop, NULL);
#endif
}

void Fin_Graphique (void)
{
  assert (initialized);

  XFreePixmap (d, bkp);
  XFreeGC (d, gc);
  XDestroyWindow (d, w);
  XFlush (d);
  XCloseDisplay (d);

  initialized = 0;
}

void Efface (void)
{
  assert (initialized);

  /* Traçage d'un rectangle blanc */

  XSetForeground (d, gc, WhitePixel (d, s));
  XFillRectangle(d, w, gc, 0, 0, w_width, w_height);
  XFillRectangle(d, bkp, gc, 0, 0, w_width, w_height);
  XFlush (d);
}

void Trait (float x1, 
	    float y1, 
	    float x2, 
	    float y2, 
	    int couleur)
{
  float _x1 = x1 - _xmin;
  float _y1 = _ymax - y1;
  float _x2 = x2 - _xmin;
  float _y2 = _ymax - y2;

  assert (initialized);

  XSetForeground (d, gc, TableCoul [couleur].pixel);

  XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
  XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  XFlush (d);
  
}

void Croix (float x, 
	    float y, 
	    int couleur)
{
  float _x = x - _xmin;
  float _y = _ymax - y;
  int dim = 3;

  assert (initialized);

  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawLine (d, w, gc, _x - dim, _y - dim, _x + dim, _y + dim);
  XDrawLine (d, bkp, gc, _x - dim, _y - dim, _x + dim, _y + dim);
  XDrawLine (d, w, gc, _x - dim, _y + dim, _x + dim, _y - dim);
  XDrawLine (d, bkp, gc, _x - dim, _y + dim, _x + dim, _y - dim);
  XFlush (d);
}

void Point (float x, 
	    float y, 
	    int couleur)
{
  float _x = x - _xmin;
  float _y = _ymax - y;

  assert (initialized);

  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawPoint(d, w, gc, _x, _y);
  XDrawPoint(d, bkp, gc, _x, _y);
  XFlush (d);
}

void Cercle (float x, 
	     float y, 
	     int rayon, 
	     int couleur)
{
  float _x = x - _xmin;
  float _y = _ymax - y;

  assert (initialized);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawArc(d, w, gc,
	   _x - rayon, _y - rayon,  /* Point de départ */ 
	   2 * rayon, 2 * rayon,    /* Point d'arrivée */
	   0, 360*64);              /* Angles (en 64e de degré) */
  XDrawArc(d, bkp, gc,
	   _x - rayon, _y - rayon,  /* Point de départ */ 
	   2 * rayon, 2 * rayon,    /* Point d'arrivée */
	   0, 360*64);              /* Angles (en 64e de degré) */
  XFlush (d);
}

void Disque (float x, 
	     float y, 
	     int rayon, 
	     int couleur)
{
  float _x = x - _xmin;
  float _y = _ymax - y;

  assert (initialized);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillArc(d, w, gc,
	   _x - rayon, _y - rayon,  /* Point de départ */ 
	   2 * rayon, 2 * rayon,    /* Point d'arrivée */
	   0, 360*64);              /* Angles (en 64e de degré) */
  XFillArc(d, bkp, gc,
	   _x - rayon, _y - rayon,  /* Point de départ */ 
	   2 * rayon, 2 * rayon,    /* Point d'arrivée */
	   0, 360*64);              /* Angles (en 64e de degré) */
  XFlush (d);
}

void DrawPoly (int nbr_points, 
	       float *poly_points, 
	       int couleur)
{
  int i;
  float _x1, _x2, _y1, _y2;
  
  assert (initialized);
  assert (nbr_points > 1);

  XSetForeground (d, gc, TableCoul [couleur].pixel);

  for (i = 0; i < nbr_points - 1; i++) {
    /* Tracer une ligne entre le point i et le point i + 1 */
    _x1 = poly_points [2 * i] - _xmin;
    _y1 = _ymax - poly_points [2 * i + 1];
    _x2 = poly_points [2 * i + 2] - _xmin;
    _y2 = _ymax - poly_points [2 * i + 3];
    XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
    XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  }

  /* Relier le dernier point au premier point */
  
  _x1 = poly_points [2 * (nbr_points - 1)] - _xmin;
  _y1 = _ymax - poly_points [2 * nbr_points - 1];
  _x2 = poly_points [0] - _xmin;
  _y2 = _ymax - poly_points [1];
  XDrawLine (d, w, gc, _x1, _y1, _x2, _y2);
  XDrawLine (d, bkp, gc, _x1, _y1, _x2, _y2);
  
  XFlush (d);  
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
    pol [i].x = poly_points [2 * i] - _xmin;
    pol [i].y = _ymax - poly_points [2 * i + 1];
  }

  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillPolygon (d, w, gc, pol, nbr_points, Complex, CoordModeOrigin);
  XFillPolygon (d, bkp, gc, pol, nbr_points, Complex, CoordModeOrigin);
  XFlush (d);

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
  float _x1 = x1 - _xmin,
    _x2 = x2 - _xmin,
    _y1 = _ymax - y1,
    _y2 = _ymax - y2;

  float _x_M = MAX (_x1, _x2),
    _x_m = MIN (_x1, _x2),
    _y_M = MAX (_y1, _y2),
    _y_m = MIN (_y1, _y2);
    
  assert (initialized);

  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawRectangle (d, w, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));
  XDrawRectangle (d, bkp, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));  
  XFlush (d);
}

void FillRectangle (float x1, 
		    float y1, 
		    float x2, 
		    float y2, 
		    int couleur)
{
  float _x1 = x1 - _xmin,
    _x2 = x2 - _xmin,
    _y1 = _ymax - y1,
    _y2 = _ymax - y2;
  
  float _x_M = MAX (_x1, _x2),
    _x_m = MIN (_x1, _x2),
    _y_M = MAX (_y1, _y2),
    _y_m = MIN (_y1, _y2);
    
  assert (initialized);
  
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XFillRectangle (d, w, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));
  XFillRectangle (d, bkp, gc, _x_m, _y_m, (_x_M - _x_m), (_y_M - _y_m));  
  XFlush (d);
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
    
  XAllocColor (d, colmap, &TableCoul [numero]);
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
  int _x = x - _xmin,
    _y = _ymax - y;
  
  assert (initialized);
  XSetForeground (d, gc, TableCoul [couleur].pixel);
  XDrawString (d, w, gc, _x, _y, chaine, strlen (chaine));
  XDrawString (d, bkp, gc, _x, _y, chaine, strlen (chaine));
  XFlush (d);
}

void Ecrit (char *chaine,
	    int couleur)
{
  EcritXY (_cur_x, _cur_y, chaine, couleur);
}

void Attente (void)
{
  char c = '\0';

  assert (initialized);
  
  while (c != '\n') {
    scanf ("%c", &c);
  }
}

void main_loop (void) {
  XEvent e;

  while (initialized) {
    XNextEvent (d, &e);
    
    if (e.type == Expose) {
      /* Si une partie de la fenêtre doit être redessinée, on la copie
	 depuis la table de pixel de sauvegarde. */

      XCopyArea (d, bkp, w, gc,
		 e.xexpose.x, e.xexpose.y,
		 e.xexpose.width, e.xexpose.height,
		 e.xexpose.x, e.xexpose.y);
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

  int steps [42] = {0, 6, 12, 18, 25, 31, 37, 43, 50, 56, 62, 68, 75, 81, 
		    87, 93, 100, 106, 112, 118, 125, 131, 137, 143, 150, 156,
		    162, 168, 175, 181, 187, 193, 200, 206, 212, 218, 225, 
		    231, 237, 243, 249, 255};

  enum p_states {i_green, s_green_i_red, 
		 s_red_d_green, s_red_i_blue, 
		 s_blue_d_red, s_blue_i_green};

  enum p_states cur_state = i_green;
  int count, step;

  colmap = DefaultColormap (d, s);
  
  for (i = 0; i < 16; i++) {
    XAllocNamedColor (d, colmap, couleursPredef [i], &TableCoul [i], &exact);
  }

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

  if (font_info != NULL) {
    XFreeFont (d, font_info);
  }

  font_info = XLoadQueryFont(d, the_font_name);

  if (font_info == NULL) {
    fprintf(stderr, "XLoadQueryFont: échec du chargement de"
	    " la police de caractère '%s'\n", the_font_name);
    exit (1);
  }
  else {
    XSetFont(d, gc, font_info->fid);
  }  
}
