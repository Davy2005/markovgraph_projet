#ifndef __HASSE_H__
#define __HASSE_H__
#include "utils.h"

// Un lien entre deux classes (from -> to)
typedef struct {
    int from;
    int to;
} t_link;

// Tableau dynamique de liens
typedef struct {
    t_link *links;
    int log_size; // nombre de liens utilisés
    int capacity; // capacité allouée
} t_link_array;

// Supprime les liens transitifs dans le Hasse ( Fonction déjà donné dans le sujet )
void removeTransitiveLinks(t_link_array *);

// Construit les liens entre classes à partir du graphe
t_link_array buildClassLinks(adj_list *, tarjan_partition *);


// Export du diagramme de Hasse en Mermaid
void convertHasseToMermaid(tarjan_partition *, t_link_array *, const char *);

// Affiche : transitoire/persistante, absorbant, irréductible
void displayGraphSpecificity(tarjan_partition *, t_link_array *);


/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif
