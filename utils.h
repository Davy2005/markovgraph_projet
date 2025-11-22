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

// =====================
// Partie 2 : Tarjan
// =====================

// Infos Tarjan pour un sommet
typedef struct {
    int id;        // identifiant du sommet (1..N)
    int index;     // numéro temporaire (ordre de visite Tarjan)
    int lowlink;   // numero accessible (plus petit index atteignable)
    int onstack;   // 1 si dans la pile de Tarjan, 0 sinon
} tarjan_vertex;

// Une composante fortement connexe (classe)
typedef struct {
    int *vertices; // sommets de la classe
    int size;      // nombre de sommets
    int capacity;  // capacité allouée
    char name[10]; // nom de la classe, ex: "C1"
} tarjan_class;

// Ensemble des classes trouvées par Tarjan
typedef struct {
    tarjan_class *classes;
    int size;      // nombre de classes utilisées
    int capacity;  // capacité allouée
} tarjan_partition;

// Petite pile pour l'algo de Tarjan
typedef struct {
    int *data;
    int top;
    int capacity;
} stack;

// Parcours récursif de Tarjan (DFS)
void t_parcours(
    int,
    adj_list *,
    tarjan_vertex *,
    stack *,
    tarjan_partition *
);

// Fonctions sur la pile
void stack_init(stack *, int);
void stack_push(stack *, int);
int  stack_pop(stack *);
int  stack_empty(stack *);

// Algo principal de Tarjan
tarjan_partition tarjan(adj_list *);

// Affichage des classes
void printPartition(tarjan_partition *);

// Tableau sommet -> classe
int *computeVertexToClassTable(tarjan_partition *, adj_list *);

#endif
