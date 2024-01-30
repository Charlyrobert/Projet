// Code en C pour le traitement S 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>


// Structure pour les nœuds de l'arbre AVL basé sur la distance
typedef struct DistanceNode {
    float distance;
    struct DistanceNode *left;
    struct DistanceNode *right;
    int height;
} DistanceNode;

// Structure des noeuds de notre arbre final (trié par Max-Min)
typedef struct MaxMinNode {
    float maxMinusMin;
    struct MaxMinNode *left;
    struct MaxMinNode *right;
    int height;
    int routeID;
    float max;
    float min;
    float average;
} MaxMinNode;


// Structure pour les nœuds de l'arbre AVL basé sur le routeID (Arbre initial)
typedef struct RouteNode {
    int routeID;
    DistanceNode *distanceTree; 
    struct RouteNode *left;
    struct RouteNode *right;
    int height;
} RouteNode;


/// PARTIE ARBRE INITIAL /////

// Fonction pour parcourir l'arbre AVL distance et trouver la distance minimale
float minNode(DistanceNode *node) {
    if (node == NULL)
        return DBL_MAX; 

    // Descendre toujours vers la gauche
    while (node->left != NULL)
        node = node->left;

    // Le nœud actuel est le nœud le plus à gauche, contenant la distance minimale
    return node->distance;
}

// Fonction pour parcourir l'arbre AVL distance et trouver la distance maximale
float maxNode(DistanceNode *node) {
    if (node == NULL)
        return FLT_MIN; 

    // Descendre toujours vers la droite
    while (node->right != NULL)
        node = node->right;

    // Le nœud actuel est le nœud le plus à droite, contenant la distance maximale
    return node->distance;
}

// Fonction récursive pour calculer la somme des distances au sein d'un noeud RouteID
float distanceSum(DistanceNode *node) {
    if (node == NULL)
        return 0;

    // Somme des distances dans les sous-arbres gauche et droit
    float sumLeft = distanceSum(node->left);
    float sumRight = distanceSum(node->right);

    // Ajouter la distance du nœud actuel
    return sumLeft + sumRight + node->distance;
}

// Fonction pour compter le nombre de noeud routeID (on compte le nombre d'étape d'un trajet)
int countNodes(DistanceNode *node, int* nodeCount){
    if (node != NULL) {
        (*nodeCount)++;
        countNodes(node->left, nodeCount);
        countNodes(node->right, nodeCount);
    }
    return *nodeCount;
}

// Fonction pour calculer la moyenne des distances d'un trajet
float distanceAvg(DistanceNode *node) {

 
    int count = 0;
    count = countNodes(node, &count); //Compter le nombre d'étape

    if (count == 0)
        return 0; 

    float sum = distanceSum(node); 
    return sum / count; 
}


// Fonction pour obtenir la hauteur de l'arbre (RouteID)
int routeHeight(RouteNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}


int routeMax(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour créer un nouveau nœud
DistanceNode* newDistanceNode(float distance) {
    DistanceNode* node = (DistanceNode*) malloc(sizeof(DistanceNode));
    node->distance = distance;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

// Fonction pour obtenir la hauteur de l'arbre
int distanceHeight(DistanceNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}


int distanceMax(int a, int b) {
    return (a > b) ? a : b;
}

// Rotation droite 
DistanceNode *distanceRightRotate(DistanceNode *y) {
    DistanceNode *x = y->left;
    DistanceNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = distanceMax(distanceHeight(y->left), distanceHeight(y->right)) + 1;
    x->height = distanceMax(distanceHeight(x->left), distanceHeight(x->right)) + 1;

 
    return x;
}

// Rotation gauche 
DistanceNode *distanceLeftRotate(DistanceNode *x) {
    DistanceNode *y = x->right;
    DistanceNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = distanceMax(distanceHeight(x->left), distanceHeight(x->right)) + 1;
    y->height = distanceMax(distanceHeight(y->left), distanceHeight(y->right)) + 1;

    return y;
}

// Obtenir l'équilibre d'un nœud 
int distanceGetBalance(DistanceNode *N) {
    if (N == NULL)
        return 0;
    return distanceHeight(N->left) - distanceHeight(N->right);
}

// Fonction pour insérer un noeud dans l'arbre des étapes
DistanceNode* distanceInsert(DistanceNode* node, float distance) {
    // Étape 1 : Insertion BST normale
    if (node == NULL)
        return newDistanceNode(distance);

    if (distance < node->distance)
        node->left = distanceInsert(node->left, distance);
    else if (distance > node->distance)
        node->right = distanceInsert(node->right, distance);
    else
        return node; 

    // Mettre à jour la hauteur 
    node->height = distanceMax(distanceHeight(node->left), distanceHeight(node->right)) + 1;

    // Obtenir le facteur d'équilibre de ce nœud pour vérifier s'il est déséquilibré
    int balance = distanceGetBalance(node);

    // Gérer les cas de déséquilibre

    // Cas gauche gauche
    if (balance > 1 && distance < node->left->distance)
        return distanceRightRotate(node);

    // Cas gauche droite
    if (balance > 1 && distance > node->left->distance) {
        node->left = distanceLeftRotate(node->left);
        return distanceRightRotate(node);
    }

    // Cas droite droite
    if (balance < -1 && distance > node->right->distance)
        return distanceLeftRotate(node);

    // Cas droite gauche
    if (balance < -1 && distance < node->right->distance) {
        node->right = distanceRightRotate(node->right);
        return distanceLeftRotate(node);
    }

    return node;
}


RouteNode* newRouteNode(int routeID, float distance) {
    RouteNode* newNode = (RouteNode*)malloc(sizeof(RouteNode));
    if (newNode == NULL) {

        fprintf(stderr, "Échec de l'allocation mémoire pour le nouveau nœud RouteNode\n");
        exit(1); 
    }
    
    newNode->routeID = routeID;
    newNode->distanceTree = NULL; 
    newNode->distanceTree = distanceInsert(newNode->distanceTree, distance); 
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; 
    
    return newNode;
}

// Rotation droite 
RouteNode *routeRightRotate(RouteNode *y) {
    RouteNode *x = y->left;
    RouteNode *T2 = x->right;


    x->right = y;
    y->left = T2;

    
    y->height = routeMax(routeHeight(y->left), routeHeight(y->right)) + 1;
    x->height = routeMax(routeHeight(x->left), routeHeight(x->right)) + 1;


    return x;
}

// Rotation gauche 
RouteNode *routeLeftRotate(RouteNode *x) {
    RouteNode *y = x->right;
    RouteNode *T2 = y->left;

 
    y->left = x;
    x->right = T2;

    x->height = routeMax(routeHeight(x->left), routeHeight(x->right)) + 1;
    y->height = routeMax(routeHeight(y->left), routeHeight(y->right)) + 1;

    return y;
}

// Obtenir l'équilibre d'un nœud
int routeGetBalance(RouteNode *N) {
    if (N == NULL)
        return 0;
    return routeHeight(N->left) - routeHeight(N->right);
}

// Fonction pour insérer un noeud (RouteID)
RouteNode* routeInsert(RouteNode* node, int routeID, float distance) {
    if (node == NULL)
        return newRouteNode(routeID,distance);

    if (routeID < node->routeID)
        node->left = routeInsert(node->left, routeID, distance);
    else if (routeID > node->routeID)
        node->right = routeInsert(node->right, routeID, distance);
    else {
        // Le routeID existe déjà, alors on insère la distance dans l'arbre AVL basé sur la distance
        node->distanceTree = distanceInsert(node->distanceTree, distance);
        return node; // Aucun besoin de rééquilibrer car le routeID n'a pas changé
    }

    // Mettre à jour la hauteur de ce nœud ancêtre
    node->height = routeMax(routeHeight(node->left), routeHeight(node->right)) + 1;

    // Obtenir le facteur d'équilibre de ce nœud pour vérifier s'il est déséquilibré
    int balance = routeGetBalance(node);

    // Gérer les cas de déséquilibre

    // Cas gauche gauche
    if (balance > 1 && routeID < node->left->routeID)
        return routeRightRotate(node);

    // Cas gauche droite
    if (balance > 1 && routeID > node->left->routeID) {
        node->left = routeLeftRotate(node->left);
        return routeRightRotate(node);
    }

    // Cas droite droite
    if (balance < -1 && routeID > node->right->routeID)
        return routeLeftRotate(node);

    // Cas droite gauche
    if (balance < -1 && routeID < node->right->routeID) {
        node->right = routeRightRotate(node->right);
        return routeLeftRotate(node);
    }


    return node;
}

//// FIN PARTIE ABRE INITIAL //////


//// DEBUT PARTIE ARBRE FINAL ////


// Fonction pour obtenir la hauteur de l'arbre AVL basé sur le routeID
int MinMaxHeight(MaxMinNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}


// Fonction pour obtenir le maximum de deux nombres
int MinMaxMax(int a, int b) {
    return (a > b) ? a : b;
}

// Obtenir l'équilibre d'un nœud de l'arbre AVL basé sur la distance
int MinMaxGetBalance(MaxMinNode *N) {
    if (N == NULL)
        return 0;
    return MinMaxHeight(N->left) - MinMaxHeight(N->right);
}

// Fonction pour effectuer une rotation à droite dans l'arbre AVL basé sur max - min
MaxMinNode *MinMaxRightRotate(MaxMinNode *y) {
    MaxMinNode *x = y->left;
    MaxMinNode *T2 = x->right;

    // Effectuer la rotation
    x->right = y;
    y->left = T2;

    // Mettre à jour les hauteurs
    y->height = MinMaxMax(MinMaxHeight(y->left), MinMaxHeight(y->right)) + 1;
    x->height = MinMaxMax(MinMaxHeight(x->left), MinMaxHeight(x->right)) + 1;

    return x;
}

// Fonction pour effectuer une rotation à gauche dans l'arbre AVL basé sur max - min
MaxMinNode *MinMaxLeftRotate(MaxMinNode *x) {
    MaxMinNode *y = x->right;
    MaxMinNode *T2 = y->left;

    // Effectuer la rotation
    y->left = x;
    x->right = T2;

    // Mettre à jour les hauteurs
    x->height = MinMaxMax(MinMaxHeight(x->left), MinMaxHeight(x->right)) + 1;
    y->height = MinMaxMax(MinMaxHeight(y->left), MinMaxHeight(y->right)) + 1;

    return y;
}


// Fonction pour insérer un maxMinusMin dans l'arbre AVL basé sur max - min
MaxMinNode* maxMinInsert(MaxMinNode* node, int routeID, float maxMinusMin, float max, float min, float average) {
    // Insertion BST normale
    if (node == NULL) {
        MaxMinNode* newNode = (MaxMinNode*)malloc(sizeof(MaxMinNode));
        if (newNode == NULL) {
            // Gérer l'échec de l'allocation mémoire
            fprintf(stderr, "Échec de l'allocation mémoire pour le nouveau nœud MaxMinNode\n");
            exit(1); // Ou gérer l'erreur de manière appropriée
        }
        newNode->routeID = routeID;
        newNode->maxMinusMin = maxMinusMin;
        newNode->max = max;
        newNode->min = min;
        newNode->average = average;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (maxMinusMin < node->maxMinusMin)
        node->left = maxMinInsert(node->left, routeID, maxMinusMin, max, min, average);
    else if (maxMinusMin > node->maxMinusMin)
        node->right = maxMinInsert(node->right, routeID, maxMinusMin, max, min, average);
    else {
        return node;
    }

    // Mettre à jour la hauteur de ce nœud ancêtre
    node->height = MinMaxMax(MinMaxHeight(node->left), MinMaxHeight(node->right)) + 1;

    // Obtenir le facteur d'équilibre de ce nœud ancêtre pour vérifier s'il est déséquilibré
    int balance = MinMaxGetBalance(node);

    // Gérer les cas de déséquilibre (rotation gauche et droite)

    // Cas gauche gauche
    if (balance > 1 && maxMinusMin < node->left->maxMinusMin)
        return MinMaxRightRotate(node);

    // Cas gauche droite
    if (balance > 1 && maxMinusMin > node->left->maxMinusMin) {
        node->left = MinMaxLeftRotate(node->left);
        return MinMaxRightRotate(node);
    }

    // Cas droite droite
    if (balance < -1 && maxMinusMin > node->right->maxMinusMin)
        return MinMaxLeftRotate(node);

    // Cas droite gauche
    if (balance < -1 && maxMinusMin < node->right->maxMinusMin) {
        node->right = MinMaxRightRotate(node->right);
        return MinMaxLeftRotate(node);
    }

    return node;
}

// Fonction pour insérer un noeud dans l'arbre final
void insertStatsInMaxMinTree(MaxMinNode** maxMinTree, RouteNode* routeNode) {

    // Calculer max, min, average et max - min pour la routeNode
    double minDist = minNode(routeNode->distanceTree);
    float maxDist = maxNode(routeNode->distanceTree);
    float avgDist = distanceAvg(routeNode->distanceTree);
    float maxMinusMin = maxDist - minDist;

    // Insérer les statistiques dans l'arbre AVL basé sur max - min
    *maxMinTree = maxMinInsert(*maxMinTree, routeNode->routeID, maxMinusMin, maxDist, minDist, avgDist);
}



// Fonction pour calculer les statistiques de distance pour chaque route et les insérer dans l'arbre AVL basé sur max - min
MaxMinNode* calculateAndInsertRouteDistanceStats(RouteNode *node, MaxMinNode* maxMinRoot) {
    if (node == NULL)
        return maxMinRoot;

    // Calculer les statistiques pour le sous-arbre gauche 
    maxMinRoot = calculateAndInsertRouteDistanceStats(node->left, maxMinRoot);

    // Calculer les statistiques pour le sous-arbre droit
    maxMinRoot = calculateAndInsertRouteDistanceStats(node->right, maxMinRoot);

    double minDist = minNode(node->distanceTree);
    float maxDist = maxNode(node->distanceTree);
    float avgDist = distanceAvg(node->distanceTree);

   
    maxMinRoot = maxMinInsert(maxMinRoot, node->routeID, maxDist - minDist, maxDist, minDist, avgDist);

    return maxMinRoot;
}


// Fonction pour sauvegarder les 50 premières valeurs dans un fichier txt
void saveLargestValuesToFile(MaxMinNode* largestValues[], int k, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s pour l'écriture\n", filename);
        return;
    }

    fprintf(file, "ID, RouteID, Max - Min, Max, Min, Avg\n");
    for (int i = 0; i < k; i++) {
        MaxMinNode* node = largestValues[i];
        fprintf(file, "%d, %d, %.2f, %.2f, %.2f, %.2f\n",
               i,node->routeID, node->maxMinusMin, node->max, node->min, node->average);
    }

    fclose(file);
}


// Fonction pour extraire les 50 valeurs qui nous intéressent 
void reverseInOrderTraversal(MaxMinNode* node, MaxMinNode* values[], int* count, int k) {
    if (node == NULL || *count >= k)
        return;

    // Parcours récursif du sous-arbre droit (valeurs plus grandes en premier)
    reverseInOrderTraversal(node->right, values, count, k);

    // Ajouter le nœud actuel à la liste des valeurs
    if (*count < k) {
        values[*count] = node;
        (*count)++;
    }

    // Parcours récursif du sous-arbre gauche
    reverseInOrderTraversal(node->left, values, count, k);
}


//// FIN PARTIE ARBRE FINAL //////




int main() {
    RouteNode *root = NULL;
    MaxMinNode* maxMinRoot = NULL;

    // Ouverture du fichier CSV
    FILE *file = fopen("data.csv", "r");
    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        return 1;
    }

    char line[1024];

    fgets(line, 1024, file);
    while (fgets(line, sizeof(line), file)) {
        int routeID, stepID;
        float distance;

         char *token = strtok(line, ";");
        if (token != NULL) {
            // Extrait la première valeur (routeID)
            sscanf(token, "%d", &routeID);

            // Passe au prochain token
            token = strtok(NULL, ";");
            if (token != NULL) {
                // Extrait la deuxième valeur (stepID)
                sscanf(token, "%d", &stepID);

                // Passe au prochain token
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                if (token != NULL) {
                    // Extrait la troisième valeur (distance)
                    sscanf(token, "%f", &distance);

                  
                    // Insérer les informations de trajet mises à jour dans l'arbre AVL
                    root = routeInsert(root, routeID, distance);
                } else {
                    printf("Erreur lors de l'analyse de la ligne : %s\n", line);
                }
            }
        }
    }

    fclose(file);
    maxMinRoot = calculateAndInsertRouteDistanceStats(root, maxMinRoot);

    MaxMinNode* largestValues[50];
    int count = 0;
    int k = 50;

    reverseInOrderTraversal(maxMinRoot, largestValues, &count, k);

    const char* filename = "50values.txt";
    // Enregistrez les 50 plus grandes valeurs dans le fichier txt
    saveLargestValuesToFile(largestValues, k, filename);

    return 0;
}
