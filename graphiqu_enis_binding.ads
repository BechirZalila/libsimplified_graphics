--  $Id$

--  Auteur: Bechir Zalila (bechir.zalila@aist.enst.fr)

--  La bibliothèque de fonctions décrites ci-dessous,
--  libgraphiqu_enis, est un clone en Ada de la bibliothèque
--  libgraphiqu réalisée à TELECOM Paristech. Elle partage avec cette
--  dernière les signatures de toutes les fonctions (sauf itos qui
--  s'appelait itoa dans l'acienne bibliothèque). Aussi, la fonction
--  'Lit' n'a pas été implantée car elle introduit des dépendances
--  jugées de trop pour le peu de fonctionnalités qu'elle ajoute. Le
--  code source de la nouvelle bibliothèque a été complétement réécrit
--  car les sources de libgraphiqu ne sont pas disponibles.

--  http://www.infres.enst.fr/~danzart/fiches/graphique/graphique_contenu.html

--  NOTE: Les noms des différentes fonctions de la bibliothèque
--  libgraphiqu (ENST) sont dans un mélange de français et d'anglais
--  qui laisse à désirer. Ils ont pourtant été gardés pour un souci de
--  retro-compatibilité.

--  Copyright (C) 2009 Bechir Zalila <bechir.zalila@aist.enst.fr>
--  Voir le fichier COPYING pour la licence

package Graphiqu_ENIS_Binding is

   Black       : constant Integer := 0;
   Blue        : constant Integer := 1;
   Green       : constant Integer := 2;
   Cyan        : constant Integer := 3;
   Red         : constant Integer := 4;
   DarkMagenta : constant Integer := 5;
   Brown       : constant Integer := 6;
   LightGray   : constant Integer := 7;
   DarkGray    : constant Integer := 8;
   LightBlue   : constant Integer := 9;
   LightGreen  : constant Integer := 10;
   LightCyan   : constant Integer := 11;
   Orange      : constant Integer := 12;
   Magenta     : constant Integer := 13;
   Yellow      : constant Integer := 14;
   White       : constant Integer := 15;

   procedure Dimensions_Fenetre (Largeur : Integer; Hauteur : Integer);
   --  Spécifie les dimensions de la fenêtre (en pixels). L'appel à
   --  cette fonction n'est nécessaire que si les coordonnées (xmin,
   --  xmax, ymin, ymax) sont trop petites ou trop grandes pour être
   --  converties en entiers et donner une fenêtres de taille
   --  raisonnable.

   procedure Initialisation_Graphique (X_Min : Float;
                                       Y_Min : Float;
                                       X_Max : Float;
                                       Y_Max : Float);
   --  Cette fonction ouvre une fenêtre graphique dont les dimensions
   --  sont délimitées par le rectangle (xmin, ymin, xmax, ymax). Les
   --  coordonnées sont des coordonnées utilisateurs (nombres
   --  réels). (xmin, ymin) est le point en bas à gauche et (xmax,
   --  ymax) est le point en haut à droite.

   procedure Fin_Graphique;
   --  L'appel de cette fonction est obligatoire pour supprimer la
   --  fenêtre graphique, faute de quoi le processus qui la gère ne
   --  s'arrêtera pas et nécessitera d'être tué.

   procedure Efface;
   --  Efface le contenu de la fenêtre.

   procedure Trait (X_1     : Float;
                    Y_1     : Float;
                    X_2     : Float;
                    Y_2     : Float;
                    Couleur : Integer);
   --  Tracé d'un trait entre les points (x1, y1) et (x2, y2) avec la
   --  couleur numéro couleur.

   procedure Croix (X       : Float;
                    Y       : Float;
                    Couleur : Integer);
   --  Tracé d'une croix autour du point de coordonnées (x, y) dans un
   --  trait de la couleur numéro couleur.

   procedure Point (X       : Float;
                    Y       : Float;
                    Couleur : Integer);
   --  Tracé du point de coordonnées (x, y) de la couleur numéro
   --  couleur.

   procedure Cercle (X       : Float;
                     Y       : Float;
                     Rayon   : Integer;
                     Couleur : Integer);
   --  Tracé d'un cercle de centre (x, y) et de rayon rayon avec la
   --  couleur numéro couleur.

   procedure Disque (X       : Float;
                     Y       : Float;
                     Rayon   : Integer;
                     Couleur : Integer);
   --  Remplissage dans la couleur numéro couleur d'un cercle de
   --  centre (x,y) et de rayon rayon.

   type Float_Array is array (Integer range <>) of Float;

   procedure Draw_Poly (Poly_Points : Float_Array;
                        Couleur     : Integer);
   --  Tracé avec la couleur numéro couleur d'un polygone dont les
   --  coordonnées sont stockées dans le tableau de réels
   --  poly_points. Le nombre de points du polygone est spécifié dans
   --  la variable nbr_points.

   procedure Fill_Poly (Poly_Points : Float_Array;
                        Couleur     : Integer);
   --  Remplissage dans la couleur couleur d'un polygone dont les
   --  coordonnées sont stockées dans le tableau de réels
   --  poly_points. Le nombre de points du polygone est spécifié dans
   --  la variable nbr_points.

   procedure Draw_Triangle (Poly_Points : Float_Array;
                            Couleur     : Integer);
   --  Tracé avec la couleur numéro couleur d'un triangle dont les
   --  coordonnées sont stockées dans le tableau de réels poly_points.

   procedure Fill_Triangle (Poly_Points : Float_Array;
                            Couleur     : Integer);
   --  Remplissage dans la couleur couleur d'un triangle dont les
   --  coordonnées sont stockées dans le tableau de réels poly_points.

   procedure Draw_Rectangle (X_1     : Float;
                             Y_1     : Float;
                             X_2     : Float;
                             Y_2     : Float;
                             Couleur : Integer);
   --  Tracé avec la couleur numéro couleur d'un rectangle déterminé
   --  par les points de coordonnées (x1, y1) et (x2, y2).

   procedure Fill_Rectangle (X_1     : Float;
                             Y_1     : Float;
                             X_2     : Float;
                             Y_2     : Float;
                             Couleur : Integer);
   --  Remplissage avec la couleur numéro couleur d'un rectangle
   --  déterminé par les points de coordonnées (x1, y1) et (x2, y2).

   procedure Create_Color_RGB (Numero : Integer;
                               R      : Integer;
                               G      : Integer;
                               B      : Integer);
   --  Changement de définition de la couleur numéro numero en lui
   --  donnant les composantes (R, G, B).

   --  Le numéro de la couleur varie entre 0 et 255.

   --  Les valeurs des composantes sont des entiers compris entre 0 et
   --  255.

   procedure Hauteur_Texte (Hauteur : Float);
   --  Change la hauteur d'écriture de texte à l'écran.

   function Itos (N : Integer) return String;
   --  Convertit l'entier n dans la chaîne de caractères s.

   procedure Deplace (X_1 : Float;
                      Y_1 : Float);
   --  On se positiojnne au point de coordonnées (x, y).

   procedure Ecrit (Chaine  : String;
                    Couleur : Integer);
   --  Ecrit la chaîne de caractères chaine avec la couleur numéro
   --  Couleur, à partir de la position courante.

   procedure Ecrit_XY (X       : Float;
                       Y       : Float;
                       Chaine  : String;
                       Couleur : Integer);
   --  Ecrit la chaîne de caractères chaine avec la couleur numéro
   --  couleur, à la position (x, y).

   procedure Attente;
   --  Bloque jusqu'à ce que l'utilistateur tape ENTREE.

   procedure Graphiqu_ENIS_Version;
   --  Affiche la version de la bibliothèque graphique et quelques
   --  informations de Copyright.

end Graphiqu_ENIS_Binding;
