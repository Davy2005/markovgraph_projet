#ifndef __UTILS_H__
#define __UTILS_H__

// =====================
// Partie 1 : graphe
// =====================

// Cellule = une arête sortante du sommet
typedef struct s_cell {
    int dest;            // sommet d'arrivée
    float proba;         // probabilité de transition
    struct s_cell *next; // arête suivante
} cell;

// Liste chaînée d'arêtes sortantes pour un sommet
typedef struct {
    cell *head;
} list;

// Liste d'adjacence = tableau de listes (une par sommet)
typedef struct {
    int size;   // nombre de sommets
    list *tab;  // tableau de listes
} adj_list;

// Création d'une cellule/arête
cell *createCell(int, float);

// Création d'une liste vide
list createList(void);

// Ajout en tête de liste
void addCellHead(list *, int, float);

// Création d'une liste d'adjacence vide
adj_list createAdjaList(int);

// Affichage d'une liste
void displaylist(list *);

// Affichage complet de la liste d'adjacence
void displayAdjaList(adj_list *);

// Lecture d'un graphe à partir d'un fichier
adj_list readGraph(const char *);

// Vérification que le graphe est bien un graphe de Markov
int verifMarkov(adj_list *);

// Génération d'un fichier au format Mermaid
void convertMermaid(adj_list *, const char *);
