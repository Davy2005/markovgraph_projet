#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char *getID(int i)
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


// Parti 1 

// Fonction sur les cellules et listes

// Créer une cellule (arête)
t_cell *createCell(int sommetdest, float prob) {
    t_cell *newCell = malloc(sizeof(t_cell));
    if (newCell == NULL) {
        perror("Erreur d’allocation mémoire !\n");
        exit(EXIT_FAILURE);
    }
    newCell->sommetdest = sommetdest;
    newCell->prob = prob;
    newCell->next = NULL;
    return newCell;
}

// Créer une liste vide
t_list createList() {
    t_list list;
    list.head = NULL;
    return list;
}

// Ajouter une cellule à une liste (insertion en tête)
void addCell(t_list *list, int sommetdest, float prob) {
    t_cell *newCell = createCell(sommetdest, prob);
    newCell->next = list->head;
    list->head = newCell;
}

// Afficher une liste
void printList(t_list *list) {
    t_cell *cur = list->head;
    while (cur != NULL) {
        printf(" -> (%d, %f)", cur->sommetdest, cur->prob);
        cur = cur->next;
    }
    printf("\n");
}

// Créer une liste d’adjacence vide (tableau de listes)
adjlist createAdjList(int size) {
    adjlist adj;
    adj.size = size;
    adj.tab = malloc(size * sizeof(t_list));
    if (adj.tab == NULL) {
        perror("Erreur d’allocation mémoire !\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        adj.tab[i] = createList(); // chaque case contient une liste vide
    }

    return adj;
}

void printAdjList(adjlist *adj) {
    for (int i = 0; i < adj->size; i++) {
        printf("Liste pour le sommet %d : [head @]", i + 1);
        t_cell *cur = adj->tab[i].head;

        // Parcours de la liste chaînée du sommet i
        while (cur != NULL) {
            printf(" -> (%d, %.2f)", cur->sommetdest, cur->prob);
            cur = cur->next;
        }

        printf("\n");
    }
}

adjlist readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;

    adjlist adj;  // déclaration de la variable pour la liste d’adjacence

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    adj = createAdjList(nbvert);  
    // Initialiser une liste d’adjacence vide à partir du nombre de sommets

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {

        // Ajouter l’arête avec conversion vers indices internes (0-based)
        addCell(&adj.tab[depart-1], arrivee, proba);
    }

    fclose(file);
    return adj;   // return la liste d’adjacence remplit
}


// Vérifier si la somme des probabilités sortantes d'un sommet est égal à 1
// Dit si c'est bien un graphe de Markov


int verifGraphDeMarkov(adjlist *liste) {
    int i;
    int estMarkov = 1; // on suppose que tout est correct au début

    // Première boucle : on vérifie si tous les sommets sont corrects
    for (i = 0; i < liste->size; i++) {
        float somme = 0.0;
        t_cell *cour = liste->tab[i].head;

        while (cour != NULL) {
            somme += cour->prob;
            cour = cour->next;
        }

        // somme doit être comprise entre 0.99 et 1.00
        if (somme < 0.99 || somme > 1.0) {
            estMarkov = 0;
        }
    }

    // Si tout est correct
    if (estMarkov == 1) {
        printf("Le graphe est un graphe de Markov\n");
        return 1;
    }

    // Sinon, message global
    printf("Le graphe n'est pas un graphe de Markov\n");

    // Et on affiche les sommets problématiques
    for (i = 0; i < liste->size; i++) {
        float somme = 0;
        t_cell *cour = liste->tab[i].head;

        while (cour != NULL) {
            somme += cour->prob;
            cour = cour->next;
        }

        if (somme < 0.99 || somme > 1.00) {
            printf("la somme des probabilites du sommet %d est %f\n", i + 1, somme);
        }
    }

    return 0;
}

// Fonction pour exporter un graphe au format Mermaid
void creerMermaid(adjlist *g, const char *filename) {

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erreur lors de la création du fichier mermaid\n");
        return;
    }

    // En-tête Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, " layout: elk\n");
    fprintf(f, " theme: neo\n");
    fprintf(f, " look: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart LR\n");

    // Sommets
    for (int i = 0; i < g->size; i++) {
        fprintf(f, "%s((%d))\n", getID(i+1), i+1);
    }

    // Arêtes
    for (int i = 0; i < g->size; i++) {

        t_cell *cour = g->tab[i].head;

        char sommetdepart[10];
        strcpy(sommetdepart, getID(i+1)); // On est forcé à utiliser strcpy pour les 2 getID ( source et arrivé ) car getID fonctionne pas deux fois sur la même fonction, la valeur est écrasée.

        while (cour != NULL) {

            char sommetarrivee[10];
            strcpy(sommetarrivee, getID(cour->sommetdest));

            fprintf(f, "%s -->|%.2f| %s\n", sommetdepart, cour->prob, sommetarrivee);

            cour = cour->next;
        }
    }

    fclose(f);
}






