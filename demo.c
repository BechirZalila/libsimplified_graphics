/* $Id$ */

/* Fichier de démonstration de la bibliothèque graphique simplifiée de
   l'ENIS. Fortement inspiré de la démonstration de la bibliothèque
   graphique de l'ENST:

   http://perso.telecom-paristech.fr/~danzart/fiches/graphique.html
*/

#include <stdio.h>
#include "graphiqu_enis.h"
#include <string.h>

int main(void) { 

  int n,i;
  char a[3];

  /* Definition de tableaux de reels qui contiennent les coordonnees
     (x,y) des points des polygones a tracer. */

  float polygone[18]={150,70,170,80,180,100,190,80,210,70,190,60,180,40,170,60};
  float triangle1[6]={150,150,220,250,170,240};
  float triangle2[6]={200,300,50,150,150,150};

  /* Ouverture d'une fenetre graphique definissant les coordonnees
     utilisateur. */

  Initialisation_Graphique(0,0,400,500);

  DrawRectangle(0,0,400,500,Red);
  DrawRectangle(10,10,390,435,Blue);
  
  /* Affichage d'un message*/

  Deplace(50,470);
  Hauteur_Texte (30.0);
  Ecrit("Dessins et couleurs", Orange);

  Hauteur_Texte(13.0);
  EcritXY(150,450,"Textes", LightBlue);

  /* Trace et Remplissage du premier polygone */

  DrawPoly(8,polygone,Orange);
  FillPoly(8,polygone,LightCyan);

  /* Trace et Remplissage d'un rectangle*/

  DrawRectangle(160,10,200,40,Blue);
  FillRectangle(160,10,200,40,LightBlue);

  /* Trace et Remplissage d'un cercle*/

  Cercle(180,300,10,Green);
  Disque(180,300,10,LightGreen);

  /* Affichage des 16 premieres couleurs*/

  Hauteur_Texte(10.0);
  for (n=0;n<16;n++){
    FillRectangle(360,50+20*n,375,50+20*n+15,n);
    DrawRectangle(360,50+20*n,375,50+20*n+15,Black);
    i=n;
    itos(i,a);
    EcritXY(340,57+20*n,a,Black);
  }

  /* Affichage des 256 couleurs disponibles */

  for (n=0;n<256;n++)
    Trait(50+n,400,50+n,430,n);

  for (n=0;n<256;n++){
    FillTriangle(triangle2,n);
  }

  /* Modification des composantes (Rouge,Vert,Bleu) de la couleur
     255. */

  CreateColorRGB(255,240,240,0);

  /* Trace et remplissage dans cette couleur du triangle tab2 */
  
  FillTriangle(triangle1,255);
  DrawTriangle(triangle1,LightBlue);
  
  Attente ();
  
  Fin_Graphique();

  printf("Fin du travail\n");
  return 0;
}
