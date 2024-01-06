#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Code en C pour le Traitement T:

typedef struct Ville {
    char nom[100];
    int voyages;
    int depart;
    struct Ville *gauche;
    struct Ville *droite;
    int hauteur;
} Ville;

typedef struct {
    char nom[100];
    int totalVoyages;
} DixVillesPlusFrequentes;



Ville* nouvelleVille(char *nom) {
    Ville *nouvelleVille = (Ville*)malloc(sizeof(Ville));
    strcpy(nouvelleVille->nom, nom);
    nouvelleVille->voyages = 0;
    nouvelleVille->depart = 0;
    nouvelleVille->gauche = nouvelleVille->droite = NULL;
    nouvelleVille->hauteur = 1;
    return nouvelleVille;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int hauteur(Ville *node) {
    if (node == NULL) {
        return 0;
    }
    return node->hauteur;
}

int getBalance(Ville *node) {
    if (node == NULL) {
        return 0;
    }
    return hauteur(node->gauche) - hauteur(node->droite);
}

Ville* rotationDroite(Ville *y) {
    Ville *x = y->gauche;
    Ville *T2 = x->droite;

    x->droite = y;
    y->gauche = T2;

    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

    return x;
}

Ville* rotationGauche(Ville *x) {
    Ville *y = x->droite;
    Ville *T2 = y->gauche;

    y->gauche = x;
    x->droite = T2;

    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

    return y;
}

Ville* insererVille(Ville* node, char *nom) {
    if (node == NULL) {
        return nouvelleVille(nom);
    }

    int compareResult = strcmp(nom, node->nom);

    if (compareResult < 0) {
        node->gauche = insererVille(node->gauche, nom);
    } else if (compareResult > 0) {
        node->droite = insererVille(node->droite, nom);
    } else {
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

void mettreÀJourCompteurs(Ville* racine, char *villeDepart, char *villeArrivee) {
  Ville *courante = racine;
  Ville *depart = NULL, *arrivee = NULL;

  while (courante != NULL) {
      int comparaisonDepart = strcmp(villeDepart, courante->nom);
      int comparaisonArrivee = strcmp(villeArrivee, courante->nom);

      if (comparaisonDepart == 0) {
          depart = courante;
      }
      if (comparaisonArrivee == 0) {
          arrivee = courante;
      }

      if (depart != NULL && arrivee != NULL) {
          break;
      }

      if (comparaisonDepart < 0) {
          courante = courante->gauche;
      } else {
          courante = courante->droite;
      }
  }

  if (depart != NULL && arrivee != NULL) {
      depart->depart++;
      arrivee->voyages++;
  }

}

void parcourir(Ville* node, DixVillesPlusFrequentes dixPlusFrequentes[], int *compteurTop) {
    if (node == NULL || *compteurTop >= 10) {
        return;
    }

    parcourir(node->gauche, dixPlusFrequentes, compteurTop);

    int i;
    for (i = 0; i < *compteurTop; ++i) {
        if (strcmp(node->nom, dixPlusFrequentes[i].nom) == 0) {
            dixPlusFrequentes[i].totalVoyages += node->voyages;
            break;
        }
    }

    if (i == *compteurTop) {
        strcpy(dixPlusFrequentes[*compteurTop].nom, node->nom);
        dixPlusFrequentes[*compteurTop].totalVoyages = node->voyages;
        (*compteurTop)++;
    }

    parcourir(node->droite, dixPlusFrequentes, compteurTop);
}


void obtenirTop10Villes(Ville* racine) {
    DixVillesPlusFrequentes dixPlusFrequentes[10];
    int compteurTop = 0;
    parcourir(racine, dixPlusFrequentes, &compteurTop);
    
    typedef struct {
        char nom[100];
        int totalVoyages;
    } DixVillesPlusFrequentes;
    
    printf("Les 10 villes les plus fréquentes :\n");
    for (int i = 0; i < compteurTop && i < 10; ++i) {
        printf("%s : %d voyages\n", dixPlusFrequentes[i].nom,   dixPlusFrequentes[i].totalVoyages);
    }
}

void traiterLigneCSV(char *ligne, Ville **racine) {
   char *token;
    token = strtok(ligne, ";");

    char villeDepart[100], villeArrivee[100];
    int distance;

    strcpy(villeDepart, token);
    token = strtok(NULL, ";");

    token = strtok(NULL, ";");
    strcpy(villeArrivee, token);
    token = strtok(NULL, ";");
    distance = atoi(token);

    *racine = insererVille(*racine, villeDepart);
    *racine = insererVille(*racine, villeArrivee);

    mettreÀJourCompteurs(*racine, villeDepart, villeArrivee);

}

int main() {
    FILE *fichier = fopen("mon_dossier_data.csv", "r");

    Ville *racine = NULL;
    char ligne[1000];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        traiterLigneCSV(ligne, &racine);
    }

    obtenirTop10Villes(racine);

    fclose(fichier);

    return 0;
}
