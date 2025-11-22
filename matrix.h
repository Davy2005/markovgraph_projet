
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "utils.h"

/* ============================
   Type de matrice n x n
   ============================ */

typedef struct {
    int n;        // dimension (n x n)
    double **m;   // m[i][j] = coefficient en ligne i, colonne j
} t_matrix;

/* Dans l'énoncé de la partie 3, on parle de t_partition :
   ici on réutilise directement la partition Tarjan. */
typedef tarjan_partition t_partition;

/* ============================
   Fonctions demandées - étape 1
   ============================ */

/**
 * Crée une matrice n x n remplie avec la valeur 0.
 */
t_matrix createZeroMatrix(int );

/**
 * Libère la mémoire associée à une matrice.
 */
void freeMatrix(t_matrix);

/**
 * Construit la matrice de transition M (n x n) à partir
 * d'une liste d'adjacence de graphe de Markov.
 */
t_matrix createMatrixFromAdjList(adj_list *);

/**
 * Affiche une matrice (utile pour la validation).
 */
void printMatrix(t_matrix);

/**
 * Recopie les valeurs de src dans dest.
 * Les deux matrices doivent avoir la même dimension.
 */
void copyMatrix(t_matrix , t_matrix );

/**
 * Effectue la multiplication de matrices carrées :
 * result = A * B.
 * Les trois matrices doivent être de même dimension.
 */
void multiplyMatrices(t_matrix , t_matrix , t_matrix );

/**
 * Calcule la "différence" entre deux matrices :
 * diff(M, N) = somme_{i,j} |M_ij - N_ij|.
 */
double diffMatrix(t_matrix , t_matrix );

/* ============================
   Fonction demandée - étape 2
   ============================ */

/**
 * Construit une sous-matrice correspondant à une composante
 * fortement connexe donnée, à partir de la matrice globale.
 *
 * - matrix : matrice de transition globale du graphe.
 * - part   : partition (Tarjan) du graphe en composantes.
 * - compo_index : indice de la composante (0..part.size-1).
 *
 * La sous-matrice ne contient que les lignes et colonnes
 * des sommets appartenant à cette composante.
 */
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);

#endif
