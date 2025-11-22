#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

/* ============================
   Matrices
   ============================ */

t_matrix createZeroMatrix(int n)
{
    t_matrix M;
    M.n = n;
    M.m = malloc(n * sizeof(double *));
    if (M.m == NULL) {
        perror("Erreur d'allocation pour les lignes de la matrice");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        M.m[i] = calloc(n, sizeof(double));  // initialise toutes les cases à 0
        if (M.m[i] == NULL) {
            perror("Erreur d'allocation pour une ligne de la matrice");
            exit(EXIT_FAILURE);
        }
    }

    return M;
}

void freeMatrix(t_matrix matrix)
{
    if (matrix.m == NULL) return;

    for (int i = 0; i < matrix.n; i++) {
        free(matrix.m[i]);
    }
    free(matrix.m);
}

void printMatrix(t_matrix matrix)
{
    for (int i = 0; i < matrix.n; i++) {
        for (int j = 0; j < matrix.n; j++) {
            printf("%6.3f ", matrix.m[i][j]);
        }
        printf("\n");
    }
}

/* ==========================================
   Liste d'adjacence à une matrice de transition
   ========================================== */

t_matrix createMatrixFromAdjList(adj_list *g)
{
    int n = g->size;
    t_matrix M = createZeroMatrix(n);

    // Pour chaque sommet i (0-based en interne, sommets 1..n dans les fichiers)
    for (int i = 0; i < n; i++) {
        cell *cur = g->tab[i].head;
        while (cur != NULL) {
            int j = cur->dest - 1;   // dest 1..n -> indice 0..n-1
            if (j >= 0 && j < n) {
                M.m[i][j] = (double)cur->proba;
            }
            cur = cur->next;
        }
    }

    return M;
}

/* ============================
   Opérations sur matrices
   ============================ */

void copyMatrix(t_matrix dest, t_matrix src)
{
    if (dest.n != src.n) {
        fprintf(stderr, "Erreur: copyMatrix, tailles incompatibles (%d vs %d)\n",
                dest.n, src.n);
        exit(EXIT_FAILURE);
    }

    int n = dest.n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest.m[i][j] = src.m[i][j];
        }
    }
}

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix result)
{
    if (A.n != B.n || A.n != result.n) {
        fprintf(stderr, "Erreur: multiplyMatrices, tailles incompatibles\n");
        exit(EXIT_FAILURE);
    }

    int n = A.n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A.m[i][k] * B.m[k][j];
            }
            result.m[i][j] = sum;
        }
    }
}

double diffMatrix(t_matrix M, t_matrix N)
{
    if (M.n != N.n) {
        fprintf(stderr, "Erreur: diffMatrix, tailles incompatibles\n");
        exit(EXIT_FAILURE);
    }

    int n = M.n;
    double diff = 0.0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            diff += fabs(M.m[i][j] - N.m[i][j]);
        }
    }

    return diff;
}

/* ============================
   Sous-matrice d'une composante
   ============================ */

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index)
{
    if (compo_index < 0 || compo_index >= part.size) {
        fprintf(stderr,
                "Erreur: subMatrix, indice de composante invalide (%d)\n",
                compo_index);
        exit(EXIT_FAILURE);
    }

    tarjan_class *C = &part.classes[compo_index];
    int k = C->size;       // nombre de sommets dans cette composante

    t_matrix sub = createZeroMatrix(k);

    // Les sommets sont stockés en 1..N dans C->vertices[]
    // On récupère les lignes/colonnes correspondantes dans la matrice globale.
    for (int i = 0; i < k; i++) {
        int vi = C->vertices[i] - 1;   // indice global 0..N-1
        for (int j = 0; j < k; j++) {
            int vj = C->vertices[j] - 1;
            sub.m[i][j] = matrix.m[vi][vj];
        }
    }

    return sub;
}
