// Code en C pour le traitement T
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour stocker les données des villes
typedef struct Ville {
  char nom[100];
  int traverses;
  struct Ville *gauche;
  struct Ville *droite;
  int hauteur;
} Ville;

// Créer une nouvelle ville
Ville *nouvelleVille(char *nom) {
  Ville *nouvelleVille = (Ville *)malloc(sizeof(Ville));
  strcpy(nouvelleVille->nom, nom);
  nouvelleVille->traverses = 1;
  nouvelleVille->gauche = nouvelleVille->droite = NULL;
  nouvelleVille->hauteur = 1;
  return nouvelleVille;
}

// Fonction pour calculer la hauteur d'un nœud dans l'arbre AVL
int hauteur(Ville *noeud) {
  if (noeud == NULL) {
    return 0;
  }
  return noeud->hauteur;
}

// Fonction pour obtenir le maximum de deux entiers
int max(int a, int b) { return (a > b) ? a : b; }

// Fonction pour faire une rotation à droite
Ville *rotationDroite(Ville *y) {
  Ville *x = y->gauche;
  Ville *T2 = x->droite;

  x->droite = y;
  y->gauche = T2;

  y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
  x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

  return x;
}

// Fonction pour faire une rotation à gauche
Ville *rotationGauche(Ville *x) {
  Ville *y = x->droite;
  Ville *T2 = y->gauche;

  y->gauche = x;
  x->droite = T2;

  x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
  y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

  return y;
}

// Obtenir le facteur d'équilibrage d'un nœud
int getBalance(Ville *noeud) {
  if (noeud == NULL) {
    return 0;
  }
  return hauteur(noeud->gauche) - hauteur(noeud->droite);
}

// Insérer une ville dans l'arbre AVL
Ville *insererVille(Ville *noeud, char *nom) {
  if (noeud == NULL) {
    return nouvelleVille(nom);
  }

  int compareResult = strcmp(nom, noeud->nom);

  if (compareResult < 0) {
    noeud->gauche = insererVille(noeud->gauche, nom);
  } else if (compareResult > 0) {
    noeud->droite = insererVille(noeud->droite, nom);
  } else {
    noeud->traverses++;
    return noeud;
  }

  noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));

  int balance = getBalance(noeud);

  if (balance > 1 && strcmp(nom, noeud->gauche->nom) < 0) {
    return rotationDroite(noeud);
  }

  if (balance < -1 && strcmp(nom, noeud->droite->nom) > 0) {
    return rotationGauche(noeud);
  }

  if (balance > 1 && strcmp(nom, noeud->gauche->nom) > 0) {
    noeud->gauche = rotationGauche(noeud->gauche);
    return rotationDroite(noeud);
  }

  if (balance < -1 && strcmp(nom, noeud->droite->nom) < 0) {
    noeud->droite = rotationDroite(noeud->droite);
    return rotationGauche(noeud);
  }

  return noeud;
}

// Fonction récursive pour parcourir l'arbre et obtenir les villes les plus traversées
void obtenirVillesPlusTraversees(Ville *noeud, Ville *villesPlusTraversees[],
                                 int *compteurTop) {
  if (noeud == NULL || *compteurTop >= 10) {
    return;
  }

  obtenirVillesPlusTraversees(noeud->gauche, villesPlusTraversees, compteurTop);

  int i;
  for (i = 0; i < *compteurTop; ++i) {
    if (strcmp(noeud->nom, villesPlusTraversees[i]->nom) == 0) {
      villesPlusTraversees[i]->traverses += noeud->traverses;
      break;
    }
  }

  if (i == *compteurTop) {
    villesPlusTraversees[*compteurTop] = noeud;
    (*compteurTop)++;
  }

  obtenirVillesPlusTraversees(noeud->droite, villesPlusTraversees, compteurTop);
}

// Fonction pour imprimer les 10 villes les plus traversées dans un fichier
void imprimerTop10Villes(FILE *fichier, Ville **villesPlusTraversees, int compteurTop) {
    fprintf(fichier, "ID, nom, traversees :\n");
    for (int i = 0; i < compteurTop && i < 10; i++) {
        fprintf(fichier, "%d,%d,%s\n", i, villesPlusTraversees[i]->traverses,villesPlusTraversees[i]->nom);
    }
}
// Fonction pour libérer la mémoire de l'arbre AVL
void libererMemoire(Ville *noeud) {
    if (noeud == NULL) {
        return;
    }

    libererMemoire(noeud->gauche);
    libererMemoire(noeud->droite);
    free(noeud);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
        printf("Usage: %s <chemin_du_fichier>\n", argv[0]);
        return 1;
    }

    const char *chemin_fichier = argv[1];
    FILE *fichier = fopen(chemin_fichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", chemin_fichier);
        return 1;
    }

  char ligne[1000];
  Ville *racine = NULL;
  printf("Debut lecture\n");
  while (fgets(ligne, sizeof(ligne), fichier)) {
    char *token;
    char depart[100], arrivee[100];
    token = strtok(ligne, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";");
    strcpy(depart, token);
    // printf("villeDepart:%s\n", depart);

    token = strtok(NULL, ";");
    strcpy(arrivee, token);
    // printf("villeArrivee:%s\n", arrivee);
    racine = insererVille(racine, depart);
    racine = insererVille(racine, arrivee);
  }

  fclose(fichier);
  printf("Fin lecture\n");

  // Créer un tableau pour stocker les villes les plus traversées
  Ville *villesPlusTraversees[10];
  int compteurTop = 0;

  obtenirVillesPlusTraversees(racine, villesPlusTraversees, &compteurTop);

  // Trier le tableau des villes les plus traversées
  for (int i = 0; i < compteurTop - 1; i++) {
    for (int j = i + 1; j < compteurTop; j++) {
      if (villesPlusTraversees[i]->traverses <
          villesPlusTraversees[j]->traverses) {
        Ville *temp = villesPlusTraversees[i];
        villesPlusTraversees[i] = villesPlusTraversees[j];
        villesPlusTraversees[j] = temp;
      }
    }
  }



  // Ouvrir le fichier en mode écriture
    fichier = fopen("../temp/t.txt", "w");

    // Vérifier si l'ouverture du fichier a réussi
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1; // Quitter le programme avec un code d'erreur
    }

    // Imprimer les 10 villes les plus traversées dans le fichier
    imprimerTop10Villes(fichier, villesPlusTraversees, sizeof(villesPlusTraversees) / sizeof(villesPlusTraversees[0]));

    // Fermer le fichier
    fclose(fichier);
    libererMemoire(racine);

  return 0;
}
