/* $Id$ */

/*

  Copyright (C) 2009 Bechir Zalila <bechir.zalila@aist.enst.fr>
  Voir le fichier COPYING pour la licence

*/

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

  /* Initialisation de l'environnement grahique avec les coordonnées
     extrêmales. */
  Initialisation_Graphique (-100, -150.7, 400.5, 300);

  /* Trace un trait entre les points (0, 0) et (50, 60) avec la
     couleur 6 (Brown) */
  Trait (0.0, 0.0, 50, 60, 6);

  /* Création d'une nouvelle couleur RGB (128, 128, 128) (Gris) et
     placement de cette couleur à la position 77. */
  CreateColorRGB (77, 128, 128, 128);

  /* Trace un trait entre les points (10, 0) et (60, 60) en utilisant
     la nouvelle couleur, 77. */
  Trait (10.0, 0.0, 60, 60, 77);

  /* Affichage d'une crois au point (0.5, 150) en utilisant la couleur
     noire (Black) */
  Croix (0.5, 150, Black);

  /* Affichage de 16 points avec les 16 premières couleurs */
  for (i = 0; i < 16; i++) {
    Point (100 + i * 10, 200.0, i);
  }

  /* Affichange de 16 cercles avec les 16 premieres couleurs et de 16
     autres disques avec un ordre de couleurs inverse. */
  for (i = 0; i < 16; i++) {
    Cercle (-80.0 + i * 30, 230.0, 15, i);
    Disque (-80.0 + i * 30, 260.0, 15, i);
  }

  /* Traçage d'un polygone en vert clair */
  
  DrawPoly (6, poly_1, LightGreen);

  /* Remplissage d'un second polygone légèrement plus petit que le
     premier, avec la couleur verte */
  FillPoly (6, poly_2, Green);

  /* Traçage de 16 triangles avec les 16 premières couleurs et
     remplissages de 16 autres triangles légèrement plus petis avec un
     ordre de couleurs inverse. */

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

  /* Traçage de 16 carrés avec les 16 premières couleurs et
     remplissages de 16 rectangles inscrits dans les carrés avec un
     ordre de couleurs inverse. */

  for (i = 0; i < 16; i++) {
    DrawRectangle (-90.0 + 30 * i, -120.0, -60.0 + 30 * i, -90.0, i);
    FillRectangle (-85.0 + 30 * i, -115.0, -65.0 + 30 * i, -90.0, 15 - i);
  }

  /* Affichage de quelques textes avec des tailles et des couleurs
     différentes. Traçage de rectangles ayant la même hauteur du texte
     à chaque fois. */
  
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
 
  /* Attente de la touche ENTREE de l'utilisateur */
  Attente ();

  /* Efface le contenu de la fenêtre */
  Efface ();

  /* Une seconde attente */
  Attente ();

  /* Destruction de la fenêtre graphique */
  Fin_Graphique ();

  return 0;
}
