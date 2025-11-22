#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}


// =====================
// Partie 1 : listes + graphe
// =====================

//Creer une cellule
cell *createCell(int destination, float prob)
{
    cell *newcell = malloc(sizeof(cell));
    if (newcell == NULL) {
        printf("Erreur : allocation memoire cellule\n");
        exit(EXIT_FAILURE);
    }

    newcell->dest = destination;
    newcell->proba = prob;
    newcell->next = NULL;
    return newcell;
}

//Cree une liste chainee simple avec un head
list createList(void)
{
    list l;
    l.head = NULL;
    return l;
}

//Affiche la liste
void displaylist(list *l)
{
    cell *courant = l->head;
    while (courant != NULL) {
        printf(" -> (%d, %f)", courant->dest, courant->proba);
        courant = courant->next;
    }
    printf("\n");
}

//Ajoute une cellule en tete de liste
void addCellHead(list *l, int destination, float prob)
{
    cell *newCell = createCell(destination, prob);
    newCell->next = l->head;
    l->head = newCell;
}

//Cree une liste adjacente (une structure qui contient un tableau des listes chainées simples
adj_list createAdjaList(int size)
{
    adj_list adjlist;
    adjlist.size = size;
    adjlist.tab = malloc(size * sizeof(list));
    if (adjlist.tab == NULL) {
        printf("Erreur : allocation memoire liste d'adjacence\n");
        exit(EXIT_FAILURE);
    }

    // On initialise chaque entrée avec une liste vide
    for (int i = 0; i < size; i++)
        adjlist.tab[i] = createList();

    return adjlist;
}

//Affichage de la liste ajdacente
void displayAdjaList(adj_list *adj)
{
    for (int i = 0; i < adj->size; i++) {
        printf("Liste pour le sommet %d : [head @]", i + 1);
        cell *cur = adj->tab[i].head;

        while (cur != NULL) {
            printf(" -> (%d, %.2f)", cur->dest, cur->proba);
            cur = cur->next;
        }

        printf("\n");
    }
}

//Lecture d'un fichier de donnée et la retranscrit dans une adjacente liste 
adj_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;

    adj_list adj;  // déclaration de la variable pour la liste d’adjacence

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    adj = createAdjaList(nbvert);  
    // Initialiser une liste d’adjacence vide à partir du nombre de sommets

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {

        // Ajouter l’arête avec conversion vers indices internes (0-based)
        addCellHead(&adj.tab[depart-1], arrivee, proba);
    }

    fclose(file);
    return adj;   // return la liste d’adjacence remplit
}

//Vérification de si c'est bien un graph de Markov
int verifMarkov(adj_list *liste)
{
    int markov = 1;

    // On vérifie la somme des probas pour chaque sommet
    for (int i = 0; i < liste->size; i++) {
        float total = 0.0f;
        cell *cour = liste->tab[i].head;

        while (cour != NULL) {
            total += cour->proba;
            cour = cour->next;
        }

        if (total < 0.99f || total > 1.0f)
            markov = 0;
    }

    if (markov) {
        printf("Le graphe est bien un graphe de Markov.\n");
        return 1;
    }

    printf("Le graphe n'est pas un graphe de Markov.\n");

    // On affiche les sommets qui posent problème
    for (int i = 0; i < liste->size; i++) {
        float total = 0.0f;
        cell *cour = liste->tab[i].head;

        while (cour != NULL) {
            total += cour->proba;
            cour = cour->next;
        }

        if (total < 0.99f || total > 1.0f)
            printf("Sommet %d : somme des proba = %f\n", i + 1, total);
    }

    return 0;
}

//Converti le format de donnée en format Mermaid pour construire le graph sur Mermaid
void convertMermaid(adj_list *liste, const char *nomdufichier)
{
    FILE *f = fopen(nomdufichier, "w");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier Mermaid\n");
        return;
    }

    // Petite config pour Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, " layout: elk\n");
    fprintf(f, " theme: neo\n");
    fprintf(f, " look: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart LR\n");

    // Sommets
    for (int i = 0; i < liste->size; i++)
        fprintf(f, "%s((%d))\n", getID(i + 1), i + 1);

    // Arêtes
    for (int j = 0; j < liste->size; j++) {
        cell *cour = liste->tab[j].head;

        char somdep[10];
        strcpy(somdep, getID(j + 1));

        while (cour != NULL) {
            char somarv[10];
            strcpy(somarv, getID(cour->dest));

            fprintf(f, "%s -->|%.2f|%s\n", somdep, cour->proba, somarv);

            cour = cour->next;
        }
    }

    fclose(f);
}

// =====================
// Partie 2 : Tarjan
// =====================

// Initialisation d'une pile
void stack_init(stack *s, int capacity)
{
    s->data = malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
}

void stack_push(stack *s, int v)
{
    s->data[++s->top] = v;
}

int stack_pop(stack *s)
{
    return s->data[s->top--];
}

int stack_empty(stack *s)
{
    return (s->top < 0);
}

// Creer et initialisation d'un tableau de sommets ( tarjan vertex )
tarjan_vertex *initTabTarjanVertex(adj_list *g)
{
    tarjan_vertex *T = malloc(g->size * sizeof(tarjan_vertex));
    for (int i = 0; i < g->size; i++) {
        T[i].id = i + 1;
        T[i].index = -1;
        T[i].lowlink = -1;
        T[i].onstack = 0;
    }
    return T;
}
