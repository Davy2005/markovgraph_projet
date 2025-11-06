#ifndef __HASSE_H__
#define __HASSE_H__

void removeTransitiveLinks(t_link_array *p_link_array);

// Une cellule représente une arête sortante d’un sommet
typedef struct cell {
    int sommetdest;              // sommet d’arrivée
    float prob;            // probabilité de passage
    struct cell *next;     // pointeur vers la cellule suivante
} t_cell;

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif