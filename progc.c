#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Code en C pour le Traitement T:

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
int hauteur(Ville *node) {
  if (node == NULL) {
    return 0;
  }
  return node->hauteur;
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
int getBalance(Ville *node) {
  if (node == NULL) {
    return 0;
  }
  return hauteur(node->gauche) - hauteur(node->droite);
}

// Insérer une ville dans l'arbre AVL
Ville *insererVille(Ville *node, char *nom) {
  if (node == NULL) {
    return nouvelleVille(nom);
  }

  int compareResult = strcmp(nom, node->nom);

  if (compareResult < 0) {
    node->gauche = insererVille(node->gauche, nom);
  } else if (compareResult > 0) {
    node->droite = insererVille(node->droite, nom);
  } else {
    node->traverses++;
    return node;
  }

  node->hauteur = 1 + max(hauteur(node->gauche), hauteur(node->droite));

  int balance = getBalance(node);

  if (balance > 1 && strcmp(nom, node->gauche->nom) < 0) {
    return rotationDroite(node);
  }

  if (balance < -1 && strcmp(nom, node->droite->nom) > 0) {
    return rotationGauche(node);
  }

  if (balance > 1 && strcmp(nom, node->gauche->nom) > 0) {
    node->gauche = rotationGauche(node->gauche);
    return rotationDroite(node);
  }

  if (balance < -1 && strcmp(nom, node->droite->nom) < 0) {
    node->droite = rotationDroite(node->droite);
    return rotationGauche(node);
  }

  return node;
}

// Fonction récursive pour parcourir l'arbre et obtenir les villes les plus
// traversées
void obtenirVillesPlusTraversees(Ville *node, Ville *villesPlusTraversees[],
                                 int *compteurTop) {
  if (node == NULL || *compteurTop >= 10) {
    return;
  }

  obtenirVillesPlusTraversees(node->gauche, villesPlusTraversees, compteurTop);

  int i;
  for (i = 0; i < *compteurTop; ++i) {
    if (strcmp(node->nom, villesPlusTraversees[i]->nom) == 0) {
      villesPlusTraversees[i]->traverses += node->traverses;
      break;
    }
  }

  if (i == *compteurTop) {
    villesPlusTraversees[*compteurTop] = node;
    (*compteurTop)++;
  }

  obtenirVillesPlusTraversees(node->droite, villesPlusTraversees, compteurTop);
}

int main() {
  FILE *fichier = fopen("data.csv", "r");
  if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier.\n");
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

  printf("Les 10 villes les plus traversées :\n");
  for (int i = 0; i < compteurTop && i < 10; i++) {
    printf("%s : %d traversées\n", villesPlusTraversees[i]->nom,
           villesPlusTraversees[i]->traverses);
  }

  return 0;
}
