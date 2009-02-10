/* $Id$ */

/*
   Auteur: Bechir Zalila (bechir.zalila@aist.enst.fr)
 
   La bibliothèque de fonctions décrites ci-dessous, libgraphiqu_enis,
   est un clone de la bibliothèque libgraphiqu réalisée à TELECOM
   Paristech. Elle partage avec cette dernière les signatures de
   toutes les fonctions (sauf itos qui s'appelait itoa dans l'acienne
   bibliothèque). Aussi, la fonction 'Lit' n'a pas été implantée car
   elle introduit des dépendances jugées de trop pour le peu de
   fonctionnalités qu'elle ajoute. Le code source de la nouvelle
   bibliothèque a été complétement réécrit car les sources de
   libgraphiqu ne sont pas disponibles.

   http://www.infres.enst.fr/~danzart/fiches/graphique/graphique_contenu.html
*/

#ifndef __GRAPHIQU_ENIS_H__
#define __GRAPHIQU_ENIS_H__

extern void Dimensions_Fenetre (int largeur, int hauteur);
/*
  Spécifie les dimensions de la fenêtre (en pixels). L'appel à cette
  fonction n'est nécessaire que si les coordonnées (xmin, xmax, ymin,
  ymax) sont trop petites ou trop pour être converties en entiers et
  donner une fenêtres de taille raisonnable.
 */

extern void Initialisation_Graphique (float xmin, 
				      float ymin, 
				      float xmax, 
				      float ymax);
/* 
   Cette fonction ouvre une fenêtre graphique dont les dimensions sont
   délimitées par le rectangle (xmin, ymin, xmax, ymax). Les
   coordonnées sont des coordonnées utilisateurs (nombres
   réels). (xmin, ymin) est le point en bas à gauche et (xmax, ymax)
   est le point en haut à droite.
*/

extern void Fin_Graphique (void);
/* 
   L'appel de cette fonction est obligatoire pour supprimer la fenêtre
   graphique, faute de quoi le processus qui la gère ne s'arrêtera pas
   et nécessitera d'être tué.
*/

extern void Efface (void);
/* 
   Efface le contenu de la fenêtre.
*/

extern void Trait (float x1, 
		   float y1, 
		   float x2, 
		   float y2, 
		   int couleur);
/* 
   Tracé d'un trait entre les points (x1, y1) et (x2, y2) avec la
   couleur numéro couleur.
*/

extern void Croix (float x, 
		   float y, 
		   int couleur);
/* 
   Tracé d'une croix autour du point de coordonnées (x, y) dans un
   trait de la couleur numéro couleur.
*/

extern void Point (float x, 
		   float y, 
		   int couleur);
/* 
   Tracé du point de coordonnées (x, y) de la couleur numéro couleur.
*/

extern void Cercle (float x, 
		    float y, 
		    int rayon, 
		    int couleur);
/* 
   Tracé d'un cercle de centre (x, y) et de rayon rayon avec la couleur
   numéro couleur.
*/

extern void Disque (float x, 
		    float y, 
		    int rayon, 
		    int couleur);
/* 
   Remplissage dans la couleur numéro couleur d'un cercle de centre
   (x,y) et de rayon rayon.
*/

extern void DrawPoly (int nbr_points, 
		      float *poly_points, 
		      int couleur);
/* 
   Tracé avec la couleur numéro couleur d'un polygone dont les
   coordonnées sont stockées dans le tableau de réels poly_points. Le
   nombre de points du polygone est spécifié dans la variable
   nbr_points.
*/

extern void FillPoly (int nbr_points, 
		      float *poly_points, 
		      int couleur);
/* 
   Remplissage dans la couleur couleur d'un polygone dont les
   coordonnées sont stockées dans le tableau de réels poly_points. Le
   nombre de points du polygone est spécifié dans la variable
   nbr_points.
*/

extern void DrawTriangle (float *poly_points, 
			  int couleur);
/* 
   Tracé avec la couleur numéro couleur d'un triangle dont les
   coordonnées sont stockées dans le tableau de réels poly_points.
*/

extern void FillTriangle (float *poly_points, 
			  int couleur);
/* 
   Remplissage dans la couleur couleur d'un triangle dont les
   coordonnées sont stockées dans le tableau de réels poly_points.
*/

extern void DrawRectangle (float x1, 
			   float y1, 
			   float x2, 
			   float y2, 
			   int couleur);
/* 
   Tracé avec la couleur numéro couleur d'un rectangle déterminé par
   les points de coordonnées (x1, y1) et (x2, y2).
*/

extern void FillRectangle (float x1, 
			   float y1, 
			   float x2, 
			   float y2, 
			   int couleur);
/* 
   Remplissage avec la couleur numéro couleur d'un rectangle déterminé
   par les points de coordonnées (x1, y1) et (x2, y2).
*/

enum Couleurs 
  {Black, 
   Blue, 
   Green, 
   Cyan, 
   Red,
   DarkMagenta, 
   Brown, 
   LightGray, 
   DarkGray,
   LightBlue, 
   LightGreen, 
   LightCyan, 
   Orange, 
   Magenta, 
   Yellow, 
   White};

extern void CreateColorRGB (int numero,
			    int R,
			    int G,
			    int B);
/* 
   Changement de définition de la couleur numéro numero en lui donnant
   les composantes (R, G, B).

   Le numéro de la couleur varie entre 0 et 255.

   Les valeurs des composantes sont des entiers compris entre 0 et
   255.
*/

extern void Hauteur_Texte (float hauteur);
/* 
   Change la hauteur d'écriture de texte à l'écran.
*/

extern void itos (int n, 
		  char s[]);
/* 
   Convertit l'entier n dans la chaîne de caractères s.
*/

extern void Deplace (float x1,
		     float y1);
/* 
   On se positiojnne au point de coordonnées (x, y).
*/

extern void Ecrit (char *chaine,
		   int couleur);
/* 
   Ecrit la chaîne de caractères chaine avec la couleur numéro
   couleur, à partir de la position courante.
*/

extern void EcritXY (float x, 
		     float y, 
		     char *chaine,
		     int couleur);
/* 
   Ecrit la chaîne de caractères chaine avec la couleur numéro
   couleur, à la position (x, y).
*/

extern void Attente (void);
/* 
   Bloque jusqu'à ce que l'utilistateur tape ENTREE.
*/

extern void Graphiqu_ENIS_Version (void);
/*
  Affiche la version de la bibliothèque et des informations de
  Copyright.
*/

#endif /* __GRAPHIQU_ENIS_H__ */
