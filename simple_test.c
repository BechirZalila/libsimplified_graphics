/* $Id$ */

#include <stdio.h>
#include <unistd.h>
#include "graphiqu_enis.h"

int main (void) {
  int i;
  float poly_1 [12] =
    {100, 0, 150, 150, 200, 150, 200, 200, 300, 200, 300, 0};
  float poly_2 [12] =
    {115, 10, 160, 140, 210, 140, 210, 190, 290, 190, 290, 10};
  float triangle [6];
  char size [5], msg [50];
  char tmpl [] = "Hello There @ '%s pixels'";

  Initialisation_Graphique (-100, -150.7, 400.5, 300);

  Trait (0.0, 0.0, 50, 60, 6);

  CreateColorRGB (77, 128, 128, 128);
  Trait (10.0, 0.0, 60, 60, 77);
  Croix (0.5, 150, 0);

  for (i = 0; i < 16; i++) {
    Point (100 + i * 10, 200.0, i);
  }

  for (i = 0; i < 16; i++) {
    Cercle (-80.0 + i * 30, 230.0, 15, i);
    Disque (-80.0 + i * 30, 260.0, 15, i);
  }

  DrawPoly (6, poly_1, 10);
  FillPoly (6, poly_2, 10);

  for (i = 0; i < 16; i++) {
    triangle [0] = -90.0 + 30 * i;
    triangle [1] = -90.0;
    triangle [2] = -60.0 + 30 * i;
    triangle [3] = -90.0;
    triangle [4] = -75.0 + 30 * i;
    triangle [5] = -60.5;
    DrawTriangle (triangle, i);
    triangle [0] = -85.0 + 30 * i;
    triangle [1] = -85.0;
    triangle [2] = -65.0 + 30 * i;
    triangle [3] = -85.0;
    triangle [4] = -75.0 + 30 * i;
    triangle [5] = -65.5;
    FillTriangle (triangle, 15 - i);
  }

  for (i = 0; i < 16; i++) {
    DrawRectangle (-90.0 + 30 * i, -120.0, -60.0 + 30 * i, -90.0, i);
    FillRectangle (-85.0 + 30 * i, -115.0, -65.0 + 30 * i, -90.0, 15 - i);
  }

  Hauteur_Texte (8);
  itos (8, size);
  sprintf (msg, tmpl, size);
  EcritXY (50, 40, msg, 1);
  DrawRectangle (50, 40, 150, 48, 0);

  Hauteur_Texte (12);
  itos (12, size);
  sprintf (msg, tmpl, size);
  EcritXY (50, 50, msg, 5);
  DrawRectangle (50, 50, 150, 62, 0);
  Croix (50, 50, 1);

  Hauteur_Texte (10);
  itos (10, size);
  sprintf (msg, tmpl, size);
  EcritXY (50, 65, msg, 3);
  DrawRectangle (50, 65, 150, 75, 0);

  Hauteur_Texte (15);
  itos (15, size);
  sprintf (msg, tmpl, size);
  EcritXY (50, 0, msg, 4);
  DrawRectangle (50, 0, 150, 15, 0);
 
  Attente ();

  Efface ();

  Attente ();

  Fin_Graphique ();

  return 0;
}
