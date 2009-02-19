with Graphiqu_ENIS_Binding; use Graphiqu_ENIS_Binding;

procedure Simple_Test_Ada is
   Poly_1   : Float_Array := (100.0, 0.0,
                              150.0, 150.0,
                              200.0, 150.0,
                              200.0, 200.0,
                              300.0, 200.0,
                              300.0, 0.0);
   Poly_2   : Float_Array := (115.0, 10.0,
                              160.0, 140.0,
                              210.0, 140.0,
                              210.0, 190.0,
                              290.0, 190.0,
                              290.0, 10.0);
   Triangle : Float_Array (0 .. 5);
begin
   --  Initialisation de l'environnement grahique avec les coordonnées
   --  extrêmales.

   Initialisation_Graphique (-100.0, -150.7, 400.5, 300.0);

   --  Trace un trait entre les points (0, 0) et (50, 60) avec la
   --  couleur 6 (Brown).

   Trait (0.0, 0.0, 50.0, 60.0, 6);

   --  Création d'une nouvelle couleur RGB (128, 128, 128) (Gris) et
   --  placement de cette couleur à la position 77.

   Create_Color_RGB (77, 128, 128, 128);

   --  Trace un trait entre les points (10, 0) et (60, 60) en utilisant
   --  la nouvelle couleur, 77.

   Trait (10.0, 0.0, 60.0, 60.0, 77);

   --  Affichage d'une crois au point (0.5, 150) en utilisant la
   --  couleur noire (Black).

   Croix (0.5, 150.0, Black);

   --  Affichage de 16 points avec les 16 premières couleurs

   for I in 0 .. 15 loop
      Point (100.0 + Float (I * 10), 200.0, I);
   end loop;

   --  Affichange de 16 cercles avec les 16 premieres couleurs et de
   --  16 autres disques avec un ordre de couleurs inverse.

   for I in 0 .. 15 loop
      Cercle (-80.0 + Float (I * 30), 230.0, 15, I);
      Disque (-80.0 + Float (I * 30), 260.0, 15, I);
   end loop;

   --  Traçage d'un polygone en vert clair

   Draw_Poly (Poly_1, LightGreen);

   --  Remplissage d'un second polygone légèrement plus petit que le
   --  premier, avec la couleur verte.

   Fill_Poly (Poly_2, Green);

   --  Traçage de 16 triangles avec les 16 premières couleurs et
   --  remplissages de 16 autres triangles légèrement plus petis avec
   --  un ordre de couleurs inverse.

   for I in 0 .. 15 loop
    Triangle (0) := -90.0 + Float (30 * I);
    Triangle (1) := -90.0;
    Triangle (2) := -60.0 + Float (30 * I);
    Triangle (3) := -90.0;
    Triangle (4) := -75.0 + Float (30 * I);
    Triangle (5) := -60.5;
    Draw_Triangle (Triangle, I);

    Triangle (0) := -85.0 + Float (30 * I);
    Triangle (1) := -85.0;
    Triangle (2) := -65.0 + Float (30 * I);
    Triangle (3) := -85.0;
    Triangle (4) := -75.0 + Float (30 * I);
    Triangle (5) := -65.5;
    Fill_Triangle (Triangle, 15 - I);
   end loop;

   --  Traçage de 16 carrés avec les 16 premières couleurs et
   --  remplissages de 16 rectangles inscrits dans les carrés avec un
   --  ordre de couleurs inverse.

   for I in 0 .. 15 loop
      Draw_Rectangle (-90.0 + Float (30 * I), -120.0,
                      -60.0 + Float (30 * I), -90.0,
                      I);
      Fill_Rectangle (-85.0 + Float (30 * I), -115.0,
                      -65.0 + Float (30 * I), -90.0,
                      15 - I);
   end loop;

   --  Affichage de quelques textes avec des tailles et des couleurs
   --  différentes. Traçage de rectangles ayant la même hauteur du
   --  texte à chaque fois.

  Hauteur_Texte (8.0);
  Ecrit_XY (50.0, 40.0, "Hello There @ '" & Itos (8) & " pixels'", 1);
  Draw_Rectangle (50.0, 40.0, 150.0, 48.0, 0);

  Hauteur_Texte (12.0);
  Ecrit_XY (50.0, 50.0, "Hello There @ '" & Itos (12) & " pixels'", 5);
  Draw_Rectangle (50.0, 50.0, 150.0, 62.0, 0);

  Hauteur_Texte (10.0);
  Ecrit_XY (50.0, 65.0, "Hello There @ '" & Itos (10) & " pixels'", 3);
  Draw_Rectangle (50.0, 65.0, 150.0, 75.0, 0);

  Hauteur_Texte (15.0);
  Ecrit_XY (50.0, 0.0, "Hello There @ '" & Itos (15) & " pixels'", 4);
  Draw_Rectangle (50.0, 0.0, 150.0, 15.0, 0);

   --  Attente de la touche ENTREE de l'utilisateur
   Attente;

   --  Efface le contenu de la fenêtre
   Efface;

   --  Une seconde attente
   Attente;

   --  Destruction de la fenêtre graphique
   Fin_Graphique;
end Simple_Test_Ada;
