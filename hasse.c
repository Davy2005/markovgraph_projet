#include <malloc.h>
#include <stdio.h>
#include "hasse.h"


void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

// Ajoute un lien entre deux classes si pas déjà présent
void add_link(t_link_array *A, int c1, int c2)
{
    for (int i = 0; i < A->log_size; i++)
        if (A->links[i].from == c1 && A->links[i].to == c2)
            return;

    if (A->log_size == A->capacity) {
        A->capacity *= 2;
        A->links = realloc(A->links, A->capacity * sizeof(t_link));
    }

    A->links[A->log_size].from = c1;
    A->links[A->log_size].to = c2;
    A->log_size++;
}

// Construit le graphe des classes (Hasse brut)
t_link_array buildClassLinks(adj_list *g, tarjan_partition *p)
{
    t_link_array A;
    A.log_size = 0;
    A.capacity = 8;
    A.links = malloc(8 * sizeof(t_link));

    // Tableau : sommet -> indice de classe
    int *classOf = computeVertexToClassTable(p, g);

    // On parcourt chaque sommet et ses arêtes
    for (int i = 0; i < g->size; i++) {
        int Ci = classOf[i];
        cell *cur = g->tab[i].head;

        while (cur != NULL) {
            int Cj = classOf[cur->dest - 1];
            if (Ci != Cj)
                add_link(&A, Ci, Cj);
            cur = cur->next;
        }
    }

    free(classOf);
    return A;
}

// Export du diagramme de Hasse en Mermaid
void convertHasseToMermaid(tarjan_partition *p, t_link_array *A, const char *file)
{
    FILE *f = fopen(file, "w");
    if (!f) return;

    fprintf(f, "---\nconfig:\n layout: elk\n theme: neo\n look: neo\n---\n");
    fprintf(f, "flowchart TD\n");

    // Noeuds = classes, avec la liste des sommets entre parenthèses
    for (int i = 0; i < p->size; i++) {
        fprintf(f, "%s((", p->classes[i].name);

        for (int j = 0; j < p->classes[i].size; j++) {
            fprintf(f, "%d", p->classes[i].vertices[j]);
            if (j < p->classes[i].size - 1)
                fprintf(f, ", ");
        }

        fprintf(f, "))\n");
    }

    // Arêtes entre classes
    for (int i = 0; i < A->log_size; i++) {
        fprintf(f, "%s --> %s\n",
                p->classes[A->links[i].from].name,
                p->classes[A->links[i].to].name);
    }

    fclose(f);
}

// Affiche les infos : classes transitoires/persistantes, absorbants, irréductible
void displayGraphSpecificity(tarjan_partition *p, t_link_array *A)
{
    int n = p->size;

    int transitoire[n];
    int persistante[n];

    // Par défaut : tout persistant
    for (int i = 0; i < n; i++) {
        transitoire[i] = 0;
        persistante[i] = 1;
    }

    // Si une classe a un lien sortant vers une autre, elle est transitoire
    for (int i = 0; i < A->log_size; i++) {
        int from = A->links[i].from;
        int to   = A->links[i].to;

        if (from != to) {
            transitoire[from] = 1;
            persistante[from] = 0;
        }
    }

    printf("=== Caracteristiques des classes ===\n");
    printf("\n");
    for (int c = 0; c < n; c++) {
        printf("%s : %s\n",
            p->classes[c].name,
            transitoire[c] ? "transitoire" : "persistante");
    }

    printf("\n");

    for (int c = 0; c < n; c++) {
        // Etat absorbant = classe persistante avec un seul sommet
        if (persistante[c] && p->classes[c].size == 1) {
            int v = p->classes[c].vertices[0];
            printf("Etat %d est absorbant\n", v);
            printf("\n");
        }
    }

    if (n == 1)
        printf("Le graphe est irreductible.\n");
    else {
        printf("Le graphe n'est PAS irreductible.\n");
    }
    printf("\n");
}
