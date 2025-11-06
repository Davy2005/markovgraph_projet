#ifndef __UTILS_H__
#define __UTILS_H__

// Partie 1 ***********************

//==== Déclaration des structures 

// Une cellule représente une arête sortante d’un sommet
typedef struct cell {
    int sommetdest;       // sommet d’arrivée
    float prob;           // probabilité de passage
    struct cell *next;    // pointeur vers la cellule suivante
} t_cell;

// Une liste contient toutes les arêtes sortantes d’un sommet
typedef struct {
    t_cell *head;            // tête de la liste
} t_list;

// Une liste d’adjacence représente le graphe complet
typedef struct {
    int size;              // nombre de sommets
    t_list *tab;           // tableau de listes (1 par sommet)
} adjlist;

//====== Fonction de création de cellules & listes

// Créer une cellule (arête)
t_cell *createCell(int , float );

// Créer une liste vide
t_list createList();

// Ajouter une cellule à une liste
void addCell(t_list *, int , float );

// Afficher une liste
void printList(t_list *);

// Créer une liste d’adjacence vide (tableau de listes)
adjlist createAdjList(int);

// Afficher une liste d’adjacence
void printAdjList(adjlist *);

// lire un fichier txt
adjlist readGraph (const char *);

// Vérifier si le graphe est un graphe de Markov
int verifGraphDeMarkov(adjlist *);

//Transformer un fichier de donnée de graph en fichier mermaid pour dessinner le graph
void creerMermaid(adjlist *, const char *);

#endif