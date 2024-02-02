//Code en C pour le traitement S
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

// Structure for nodes in the AVL tree based on distance
typedef struct DistanceNode {
    float distance;
    struct DistanceNode *left;
    struct DistanceNode *right;
    int height;
} DistanceNode;

// Structure for nodes in the final tree (sorted by Max-Min)
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

// Structure for nodes in the AVL tree based on routeID (Initial tree)
typedef struct RouteNode {
    int routeID;
    DistanceNode *distanceTree; 
    struct RouteNode *left;
    struct RouteNode *right;
    int height;
} RouteNode;

// Function to traverse the AVL tree based on distance and find the minimum distance
float minNode(DistanceNode *node) {
    if (node == NULL)
        return DBL_MAX; 

    // Always move to the left
    while (node->left != NULL)
        node = node->left;

    // Current node is the leftmost node, containing the minimum distance
    return node->distance;
}

// Function to traverse the AVL tree based on distance and find the maximum distance
float maxNode(DistanceNode *node) {
    if (node == NULL)
        return FLT_MIN; 

    // Always move to the right
    while (node->right != NULL)
        node = node->right;

    // Current node is the rightmost node, containing the maximum distance
    return node->distance;
}

// Recursive function to calculate the sum of distances within a RouteID node
float distanceSum(DistanceNode *node) {
    if (node == NULL)
        return 0;

    // Sum of distances in the left and right subtrees
    float sumLeft = distanceSum(node->left);
    float sumRight = distanceSum(node->right);

    // Add the distance of the current node
    return sumLeft + sumRight + node->distance;
}

// Function to count the number of RouteID nodes (counting the number of steps in a trip)
int countNodes(DistanceNode *node, int* nodeCount){
    if (node != NULL) {
        (*nodeCount)++;
        countNodes(node->left, nodeCount);
        countNodes(node->right, nodeCount);
    }
    return *nodeCount;
}

// Function to calculate the average distance of a trip
float distanceAvg(DistanceNode *node) {
    int count = 0;
    count = countNodes(node, &count); // Count the number of steps

    if (count == 0)
        return 0; 

    float sum = distanceSum(node); 
    return sum / count; 
}

// Function to get the height of the tree (RouteID)
int routeHeight(RouteNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int routeMax(int a, int b) {
    return (a > b) ? a : b;
}

// Function to create a new node
DistanceNode* newDistanceNode(float distance) {
    DistanceNode* node = (DistanceNode*) malloc(sizeof(DistanceNode));
    node->distance = distance;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

// Function to get the height of the tree
int distanceHeight(DistanceNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int distanceMax(int a, int b) {
    return (a > b) ? a : b;
}

// Right Rotation 
DistanceNode *distanceRightRotate(DistanceNode *y) {
    DistanceNode *x = y->left;
    DistanceNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = distanceMax(distanceHeight(y->left), distanceHeight(y->right)) + 1;
    x->height = distanceMax(distanceHeight(x->left), distanceHeight(x->right)) + 1;

    return x;
}

// Left Rotation 
DistanceNode *distanceLeftRotate(DistanceNode *x) {
    DistanceNode *y = x->right;
    DistanceNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = distanceMax(distanceHeight(x->left), distanceHeight(x->right)) + 1;
    y->height = distanceMax(distanceHeight(y->left), distanceHeight(y->right)) + 1;

    return y;
}

// Get the balance of a node 
int distanceGetBalance(DistanceNode *N) {
    if (N == NULL)
        return 0;
    return distanceHeight(N->left) - distanceHeight(N->right);
}

// Function to insert a node into the steps tree
DistanceNode* distanceInsert(DistanceNode* node, float distance) {
    // Step 1: Normal BST insertion
    if (node == NULL)
        return newDistanceNode(distance);

    if (distance < node->distance)
        node->left = distanceInsert(node->left, distance);
    else if (distance > node->distance)
        node->right = distanceInsert(node->right, distance);
    else
        return node; 

    // Update the height 
    node->height = distanceMax(distanceHeight(node->left), distanceHeight(node->right)) + 1;

    // Get the balance factor of this node to check if it is unbalanced
    int balance = distanceGetBalance(node);

    // Handle unbalance cases

    // Left Left Case
    if (balance > 1 && distance < node->left->distance)
        return distanceRightRotate(node);

    // Left Right Case
    if (balance > 1 && distance > node->left->distance) {
        node->left = distanceLeftRotate(node->left);
        return distanceRightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && distance > node->right->distance)
        return distanceLeftRotate(node);

    // Right Left Case
    if (balance < -1 && distance < node->right->distance) {
        node->right = distanceRightRotate(node->right);
        return distanceLeftRotate(node);
    }

    return node;
}

// Function to create a new RouteNode
RouteNode* newRouteNode(int routeID, float distance) {
    RouteNode* newNode = (RouteNode*)malloc(sizeof(RouteNode));
    if (newNode == NULL) {

        fprintf(stderr, "Memory allocation failure for new RouteNode\n");
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

// Right Rotation 
RouteNode *routeRightRotate(RouteNode *y) {
    RouteNode *x = y->left;
    RouteNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = routeMax(routeHeight(y->left), routeHeight(y->right)) + 1;
    x->height = routeMax(routeHeight(x->left), routeHeight(x->right)) + 1;

    return x;
}

// Left Rotation 
RouteNode *routeLeftRotate(RouteNode *x) {
    RouteNode *y = x->right;
    RouteNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = routeMax(routeHeight(x->left), routeHeight(x->right)) + 1;
    y->height = routeMax(routeHeight(y->left), routeHeight(y->right)) + 1;

    return y;
}

// Get the balance of a node
int routeGetBalance(RouteNode *N) {
    if (N == NULL)
        return 0;
    return routeHeight(N->left) - routeHeight(N->right);
}

// Function to insert a node (RouteID)
RouteNode* routeInsert(RouteNode* node, int routeID, float distance) {
    if (node == NULL)
        return newRouteNode(routeID,distance);

    if (routeID < node->routeID)
        node->left = routeInsert(node->left, routeID, distance);
    else if (routeID > node->routeID)
        node->right = routeInsert(node->right, routeID, distance);
    else {
        // RouteID already exists, so insert the distance into the AVL tree based on distance
        node->distanceTree = distanceInsert(node->distanceTree, distance);
        return node; // No need to rebalance as RouteID has not changed
    }

    // Update the height of this ancestor node
    node->height = routeMax(routeHeight(node->left), routeHeight(node->right)) + 1;

    // Get the balance factor of this ancestor node to check if it is unbalanced
    int balance = routeGetBalance(node);

    // Handle unbalance cases

    // Left Left Case
    if (balance > 1 && routeID < node->left->routeID)
        return routeRightRotate(node);

    // Left Right Case
    if (balance > 1 && routeID > node->left->routeID) {
        node->left = routeLeftRotate(node->left);
        return routeRightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && routeID > node->right->routeID)
        return routeLeftRotate(node);

    // Right Left Case
    if (balance < -1 && routeID < node->right->routeID) {
        node->right = routeRightRotate(node->right);
        return routeLeftRotate(node);
    }

    return node;
}

// Function to get the height of the AVL tree based on routeID
int MinMaxHeight(MaxMinNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Function to get the maximum of two numbers
int MinMaxMax(int a, int b) {
    return (a > b) ? a : b;
}

// Get the balance of a node in the AVL tree based on distance
int MinMaxGetBalance(MaxMinNode *N) {
    if (N == NULL)
        return 0;
    return MinMaxHeight(N->left) - MinMaxHeight(N->right);
}

// Function to perform right rotation in the AVL tree based on max - min
MaxMinNode *MinMaxRightRotate(MaxMinNode *y) {
    MaxMinNode *x = y->left;
    MaxMinNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = MinMaxMax(MinMaxHeight(y->left), MinMaxHeight(y->right)) + 1;
    x->height = MinMaxMax(MinMaxHeight(x->left), MinMaxHeight(x->right)) + 1;

    return x;
}

// Function to perform left rotation in the AVL tree based on max - min
MaxMinNode *MinMaxLeftRotate(MaxMinNode *x) {
    MaxMinNode *y = x->right;
    MaxMinNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = MinMaxMax(MinMaxHeight(x->left), MinMaxHeight(x->right)) + 1;
    y->height = MinMaxMax(MinMaxHeight(y->left), MinMaxHeight(y->right)) + 1;

    return y;
}

// Function to insert a maxMinusMin into the AVL tree based on max - min
MaxMinNode* maxMinInsert(MaxMinNode* node, int routeID, float maxMinusMin, float max, float min, float average) {
    // Normal BST insertion
    if (node == NULL) {
        MaxMinNode* newNode = (MaxMinNode*)malloc(sizeof(MaxMinNode));
        if (newNode == NULL) {
            // Handle memory allocation failure
            fprintf(stderr, "Memory allocation failure for new MaxMinNode\n");
            exit(1); 
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

    // Update the height of this ancestor node
    node->height = MinMaxMax(MinMaxHeight(node->left), MinMaxHeight(node->right)) + 1;

    // Get the balance factor of this ancestor node to check if it is unbalanced
    int balance = MinMaxGetBalance(node);

    // Handle unbalance cases (left and right rotation)

    // Left Left Case
    if (balance > 1 && maxMinusMin < node->left->maxMinusMin)
        return MinMaxRightRotate(node);

    // Left Right Case
    if (balance > 1 && maxMinusMin > node->left->maxMinusMin) {
        node->left = MinMaxLeftRotate(node->left);
        return MinMaxRightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && maxMinusMin > node->right->maxMinusMin)
        return MinMaxLeftRotate(node);

    // Right Left Case
    if (balance < -1 && maxMinusMin < node->right->maxMinusMin) {
        node->right = MinMaxRightRotate(node->right);
        return MinMaxLeftRotate(node);
    }

    return node;
}

// Function to insert a node into the final tree
void insertStatsInMaxMinTree(MaxMinNode** maxMinTree, RouteNode* routeNode) {

    // Calculate max, min, average, and max - min for the routeNode
    double minDist = minNode(routeNode->distanceTree);
    float maxDist = maxNode(routeNode->distanceTree);
    float avgDist = distanceAvg(routeNode->distanceTree);
    float maxMinusMin = maxDist - minDist;

    // Insert the statistics into the AVL tree based on max - min
    *maxMinTree = maxMinInsert(*maxMinTree, routeNode->routeID, maxMinusMin, maxDist, minDist, avgDist);
}

// Function to calculate distance statistics for each route and insert them into the AVL tree based on max - min
MaxMinNode* calculateAndInsertRouteDistanceStats(RouteNode *node, MaxMinNode* maxMinRoot) {
    if (node == NULL)
        return maxMinRoot;

    // Calculate statistics for the left subtree 
    maxMinRoot = calculateAndInsertRouteDistanceStats(node->left, maxMinRoot);

    // Calculate statistics for the right subtree
    maxMinRoot = calculateAndInsertRouteDistanceStats(node->right, maxMinRoot);

    double minDist = minNode(node->distanceTree);
    float maxDist = maxNode(node->distanceTree);
    float avgDist = distanceAvg(node->distanceTree);

    maxMinRoot = maxMinInsert(maxMinRoot, node->routeID, maxDist - minDist, maxDist, minDist, avgDist);

    return maxMinRoot;
}

// Function to save the top 50 values to a txt file
void saveLargestValuesToFile(MaxMinNode* largestValues[], int k, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s for writing\n", filename);
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

// Function to extract the top 50 values of interest 
void reverseInOrderTraversal(MaxMinNode* node, MaxMinNode* values[], int* count, int k) {
    if (node == NULL || *count >= k)
        return;

    // Recursive traversal of the right subtree (larger values first)
    reverseInOrderTraversal(node->right, values, count, k);

    // Add the current node to the values list
    if (*count < k) {
        values[*count] = node;
        (*count)++;
    }

    // Recursive traversal of the left subtree
    reverseInOrderTraversal(node->left, values, count, k);
}





int main(int argc, char *argv[]) {
    RouteNode *root = NULL;
    MaxMinNode* maxMinRoot = NULL;

    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Error opening file %s.\n", file_path);
        return 1;
    }

    char line[1024];

    fgets(line, 1024, file);
    while (fgets(line, sizeof(line), file)) {
        int routeID, stepID;
        float distance;

         char *token = strtok(line, ";");
        if (token != NULL) {
            // Extract the first value (routeID)
            sscanf(token, "%d", &routeID);

            // Move to the next token
            token = strtok(NULL, ";");
            if (token != NULL) {
                // Extract the second value (stepID)
                sscanf(token, "%d", &stepID);

                // Move to the next token
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                if (token != NULL) {
                    // Extract the third value (distance)
                    sscanf(token, "%f", &distance);

                  
                    // Insert the updated route information into the AVL tree
                    root = routeInsert(root, routeID, distance);
                } else {
                    printf("Error parsing line: %s\n", line);
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

    const char* filename = "../temp/s.txt";
    // Save the top 50 values to the txt file
    saveLargestValuesToFile(largestValues, k, filename);

    return 0;
}
