#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"

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


// *******************
// Partie 1
// *******************



// Création d'une cellule / arête
cell *createCell(int destination, float prob) {

    cell *newcell = malloc(sizeof(cell));
    
    if (newcell == NULL) {

        printf("Erreur : Erreur d’allocation mémoire de la cellule !\n");
        exit(EXIT_FAILURE);
    }

    newcell->dest = destination;
    newcell->proba = prob;
    newcell->next = NULL;
    return newcell;

}



// Créer une liste et initialisée à vide
list createList() {
    list list;
    list.head = NULL;
    return list;
}

// Afficher une liste
void displaylist(list *list) {
    cell *courant = list->head;
    while (courant != NULL) {
        printf(" -> (%d, %f)", courant->dest, courant->proba);
        courant = courant->next;
    }
    printf("\n");
}

// Insertion en tête de liste
void addCellHead(list *list, int destination, float prob) {
    cell *newCell = createCell(destination, prob);
    newCell->next = list->head;
    list->head = newCell;
}



// Créer une liste d’adjacence vide (tableau de listes)
adj_list createAdjaList(int size) {
    adj_list adjlist;
    adjlist.size = size;
    adjlist.tab = malloc(size * sizeof(list));
    if (adjlist.tab == NULL) {
        printf("Erreur : Erreur d'allocation mémoire d'une liste adjacente !\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        adjlist.tab[i] = createList(); // les différentes indices ont maintenant une liste vide!
    }

    return adjlist;
}



void displayAdjaList(adj_list *adj) {
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


// Check si la somme des probabilités sortantes d'un sommet est égal à 1 ( entre 0,99 et 1).
// Retourne si c'est bien un graphe de Markov


int verifMarkov(adj_list *liste) {

    int markov = 1; // on suppose que tout est correct au début
    int i;

    // On vérifie si tous les sommets sont conformes
    for (i = 0; i < liste->size; i++) {

        float total = 0.0;
        cell *cour = liste->tab[i].head;

        
        while (cour != NULL) {

            total += cour->proba;
            cour = cour->next;

        }



        // La somme des probas => doit être comprise entre 0.99 et 1.00
        if (total < 0.99 || total > 1.0) {

            markov = 0;

        }
    }

    // Si c'est bon alors on return 1 et on dit que c'est bien un graph de Markov
    if (markov == 1) {

        printf("Le graphe est bien un graphe de Markov !\n");
        return 1;

    }

    // Sinon, on dit que c'est pas un graph de Markov et on affiche les causes
    printf("Le graphe n'est pas un graphe de Markov !\n");

    // Affichage des sommets qui posent problèmes
    for (i = 0; i < liste->size; i++) {


        float total = 0;
        cell *cour = liste->tab[i].head;

        while (cour != NULL) {
            total += cour->proba;
            cour = cour->next;
        }

        if (total < 0.99 || total > 1.00) {
            printf("La somme de probabilite du sommet %d est de [%f].\n", i + 1, total);
        }
    }

    return 0;
}

// Conversion d'un fichier de donnée de graph.txt en fichier de donnee mermaid.

void convertMermaid(adj_list *liste, const char *nomdufichier) {

    FILE *f = fopen(nomdufichier, "w");
    if (f == NULL) {
        printf("Erreur dans la conversion du fichier de donnee mermaid.\n");
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
    for (int i = 0; i < liste->size; i++) {
        fprintf(f, "%s((%d))\n", getID(i+1), i+1);
    }

    // Arêtes
    for (int j = 0; j < liste->size; j=j+1) {

        cell *cour = liste->tab[j].head;

        char somdep[10];
        strcpy(somdep, getID(j+1));

        // On est forcé à utiliser strcpy pour les 2 getID ( source et arrivé ) 
        // car getID fonctionne pas deux fois sur la même fonction, la valeur est écrasée à chaque fois.


        while (cour != NULL) {


            char somarv[10];


            strcpy(somarv, getID(cour->dest));

            fprintf(f, "%s -->|%.2f|%s\n", somdep, cour->proba, somarv);

            cour = cour->next;
        }
    }

    fclose(f);
}


// **************************************************************
// PARTIE 2 – TARJAN
// **************************************************************
