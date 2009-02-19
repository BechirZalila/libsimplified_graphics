--  $Id$

with Interfaces.C; use Interfaces.C;
with System;       use System;

package body Graphiqu_ENIS_Binding is

   ------------------------
   -- Dimensions_Fenetre --
   ------------------------

   procedure Dimensions_Fenetre (Largeur : Integer; Hauteur : Integer) is
      procedure C_Dimensions_Fenetre (Largeur : Integer; Hauteur : Integer);
      pragma Import (C, C_Dimensions_Fenetre, "Dimensions_Fenetre");
   begin
      C_Dimensions_Fenetre (Largeur, Hauteur);
   end Dimensions_Fenetre;

   ------------------------------
   -- Initialisation_Graphique --
   ------------------------------

   procedure Initialisation_Graphique
     (X_Min : Float;
      Y_Min : Float;
      X_Max : Float;
      Y_Max : Float)
   is
      procedure C_Initialisation_Graphique
        (X_Min : Float;
         Y_Min : Float;
         X_Max : Float;
         Y_Max : Float);
      pragma Import (C,
                     C_Initialisation_Graphique,
                     "Initialisation_Graphique");
   begin
      C_Initialisation_Graphique (X_Min, Y_Min, X_Max, Y_Max);
   end Initialisation_Graphique;

   -------------------
   -- Fin_Graphique --
   -------------------

   procedure Fin_Graphique is
      procedure C_Fin_Graphique;
      pragma Import (C, C_Fin_Graphique, "Fin_Graphique");
   begin
      C_Fin_Graphique;
   end Fin_Graphique;

   ------------
   -- Efface --
   ------------

   procedure Efface is
      procedure C_Efface;
      pragma Import (C, C_Efface, "Efface");
   begin
      C_Efface;
   end Efface;

   -----------
   -- Trait --
   -----------

   procedure Trait
     (X_1     : Float;
      Y_1     : Float;
      X_2     : Float;
      Y_2     : Float;
      Couleur : Integer)
   is
      procedure C_Trait
        (X_1     : Float;
         Y_1     : Float;
         X_2     : Float;
         Y_2     : Float;
         Couleur : Integer);
      pragma Import (C, C_Trait, "Trait");
   begin
      C_Trait (X_1, Y_1, X_2, Y_2, Couleur);
   end Trait;

   -----------
   -- Croix --
   -----------

   procedure Croix
     (X       : Float;
      Y       : Float;
      Couleur : Integer)
   is
      procedure C_Croix
        (X       : Float;
         Y       : Float;
         Couleur : Integer);
      pragma Import (C, C_Croix, "Croix");
   begin
      C_Croix (X, Y, Couleur);
   end Croix;

   -----------
   -- Point --
   -----------

   procedure Point
     (X       : Float;
      Y       : Float;
      Couleur : Integer)
   is
      procedure C_Point
        (X       : Float;
         Y       : Float;
         Couleur : Integer);
      pragma Import (C, C_Point, "Point");
   begin
      C_Point (X, Y, Couleur);
   end Point;

   ------------
   -- Cercle --
   ------------

   procedure Cercle
     (X       : Float;
      Y       : Float;
      Rayon   : Integer;
      Couleur : Integer)
   is
      procedure C_Cercle
        (X       : Float;
         Y       : Float;
         Rayon   : Integer;
         Couleur : Integer);
      pragma Import (C, C_Cercle, "Cercle");
   begin
      C_Cercle (X, Y, Rayon, Couleur);
   end Cercle;

   ------------
   -- Disque --
   ------------

   procedure Disque
     (X       : Float;
      Y       : Float;
      Rayon   : Integer;
      Couleur : Integer)
   is
      procedure C_Disque
        (X       : Float;
         Y       : Float;
         Rayon   : Integer;
         Couleur : Integer);
      pragma Import (C, C_Disque, "Disque");
   begin
      C_Disque (X, Y, Rayon, Couleur);
   end Disque;

   ---------------
   -- Draw_Poly --
   ---------------

   procedure Draw_Poly
     (Poly_Points : Float_Array;
      Couleur     : Integer)
   is
      procedure C_Draw_Poly
        (Nbr_Points  : Integer;
         Poly_Points : Address;
         Couleur     : Integer);
      pragma Import (C, C_Draw_Poly, "DrawPoly");
   begin
      --  Il s'agit d'un tableau de coordonnées, donc le nombre de
      --  points est égal à la moitié de la taille du tableau.

      C_Draw_Poly (Poly_Points'Length / 2, Poly_Points'Address, Couleur);
   end Draw_Poly;

   ---------------
   -- Fill_Poly --
   ---------------

   procedure Fill_Poly
     (Poly_Points : Float_Array;
      Couleur     : Integer)
   is
      procedure C_Fill_Poly
        (Nbr_Points  : Integer;
         Poly_Points : Address;
         Couleur     : Integer);
      pragma Import (C, C_Fill_Poly, "FillPoly");
   begin
      --  Il s'agit d'un tableau de coordonnées, donc le nombre de
      --  points est égal à la moitié de la taille du tableau.

      C_Fill_Poly (Poly_Points'Length / 2, Poly_Points'Address, Couleur);
   end Fill_Poly;

   -------------------
   -- Draw_Triangle --
   -------------------

   procedure Draw_Triangle
     (Poly_Points : Float_Array;
      Couleur     : Integer)
   is
      procedure C_Draw_Triangle
        (Poly_Points : Address;
         Couleur     : Integer);
      pragma Import (C, C_Draw_Triangle, "DrawTriangle");
   begin
      C_Draw_Triangle (Poly_Points'Address, Couleur);
   end Draw_Triangle;

   -------------------
   -- Fill_Triangle --
   -------------------

   procedure Fill_Triangle
     (Poly_Points : Float_Array;
      Couleur     : Integer)
   is
      procedure C_Fill_Triangle
        (Poly_Points : Address;
         Couleur     : Integer);
      pragma Import (C, C_Fill_Triangle, "FillTriangle");
   begin
      C_Fill_Triangle (Poly_Points'Address, Couleur);
   end Fill_Triangle;

   --------------------
   -- Draw_Rectangle --
   --------------------

   procedure Draw_Rectangle
     (X_1     : Float;
      Y_1     : Float;
      X_2     : Float;
      Y_2     : Float;
      Couleur : Integer)
   is
      procedure C_Draw_Rectangle
        (X_1     : Float;
         Y_1     : Float;
         X_2     : Float;
         Y_2     : Float;
         Couleur : Integer);
      pragma Import (C, C_Draw_Rectangle, "DrawRectangle");
   begin
      C_Draw_Rectangle (X_1, Y_1, X_2, Y_2, Couleur);
   end Draw_Rectangle;

   --------------------
   -- Fill_Rectangle --
   --------------------

   procedure Fill_Rectangle
     (X_1     : Float;
      Y_1     : Float;
      X_2     : Float;
      Y_2     : Float;
      Couleur : Integer)
   is
      procedure C_Fill_Rectangle
        (X_1     : Float;
         Y_1     : Float;
         X_2     : Float;
         Y_2     : Float;
         Couleur : Integer);
      pragma Import (C, C_Fill_Rectangle, "FillRectangle");
   begin
      C_Fill_Rectangle (X_1, Y_1, X_2, Y_2, Couleur);
   end Fill_Rectangle;

   ----------------------
   -- Create_Color_RGB --
   ----------------------

   procedure Create_Color_RGB
     (Numero : Integer;
      R      : Integer;
      G      : Integer;
      B      : Integer)
   is
      procedure C_Create_Color_RGB
        (Numero : Integer;
         R      : Integer;
         G      : Integer;
         B      : Integer);
      pragma Import (C, C_Create_Color_RGB, "CreateColorRGB");
   begin
      C_Create_Color_RGB (Numero, R, G, B);
   end Create_Color_RGB;

   -------------------
   -- Hauteur_Texte --
   -------------------

   procedure Hauteur_Texte (Hauteur : Float) is
      procedure C_Hauteur_Texte (Hauteur : Float);
      pragma Import (C, C_Hauteur_Texte, "Hauteur_Texte");
   begin
      C_Hauteur_Texte (Hauteur);
   end Hauteur_Texte;

   ----------
   -- Itos --
   ----------

   function Itos (N : Integer) return String is
      Image : constant String := Integer'Image (N);
   begin
      if Image (Image'First) = ' ' then
         return Image (Image'First + 1 .. Image'Last);
      else
         return Image;
      end if;
   end Itos;

   -------------
   -- Deplace --
   -------------

   procedure Deplace
     (X_1 : Float;
      Y_1 : Float)
   is
      procedure C_Deplace
        (X_1 : Float;
         Y_1 : Float);
      pragma Import (C, C_Deplace, "Deplace");
   begin
      C_Deplace (X_1, Y_1);
   end Deplace;

   -----------
   -- Ecrit --
   -----------

   procedure Ecrit
     (Chaine  : String;
      Couleur : Integer)
   is
      procedure C_Ecrit (Chaine : char_array; Couleur : Integer);
      pragma Import (C, C_Ecrit, "Ecrit");
   begin
      C_Ecrit (To_C (Chaine, Append_Nul => True), Couleur);
   end Ecrit;

   --------------
   -- Ecrit_XY --
   --------------

   procedure Ecrit_XY
     (X       : Float;
      Y       : Float;
      Chaine  : String;
      Couleur : Integer)
   is
      procedure C_Ecrit_XY
        (X       : Float;
         Y       : Float;
         Chaine  : char_array;
         Couleur : Integer);
      pragma Import (C, C_Ecrit_XY, "EcritXY");
   begin
      C_Ecrit_XY (X, Y, To_C (Chaine, Append_Nul => True), Couleur);
   end Ecrit_XY;

   -------------
   -- Attente --
   -------------

   procedure Attente is
      procedure C_Attente;
      pragma Import (C, C_Attente, "Attente");
   begin
      C_Attente;
   end Attente;

   ---------------------------
   -- Graphiqu_ENIS_Version --
   ---------------------------

   procedure Graphiqu_ENIS_Version is
      procedure C_Graphiqu_ENIS_Version;
      pragma Import (C, C_Graphiqu_ENIS_Version, "Graphiqu_ENIS_Version");
   begin
      C_Graphiqu_ENIS_Version;
   end Graphiqu_ENIS_Version;

end Graphiqu_ENIS_Binding;
