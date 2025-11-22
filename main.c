#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"
#include "utils.h"
#include "matrix.h"

int main() {

// *************************
// Partie 1
// *************************
//

    printf("==============================================\n");
    printf("  TEST PARTIE 1 : GRAPH MARKOV + FICHIER MERMAID        \n");
    printf("==============================================\n\n");

    // Essai sur exemple1_chatGPT_fixed.txt

    printf("Essai sur exemple1_chatGPT_fixed.txt :\n");
    adj_list list0 = readGraph("data/exemple1_chatGPT_fixed.txt");
    displayAdjaList(&list0);
    printf("\n");
    verifMarkov(&list0);
    printf("\n");

    // Essai sur exemple1

    printf("Essai sur exemple1 :\n");
    adj_list list1 = readGraph("data/exemple1.txt");
    displayAdjaList(&list1);
    printf("\n");
    verifMarkov(&list1);
    printf("\n");


    //Essai sur les donnés de exemple2.txt

    printf("Essai sur exemple2 :\n");
    adj_list list2 = readGraph("data/exemple2.txt");
    displayAdjaList(&list2);
    printf("\n");
    verifMarkov(&list2);
    printf("\n");

    //Essai sur les donnés de exemple1_from_chatGPT.txt

    printf("Essai sur exemple1_from_chatGPT.txt :\n");
    adj_list list3 = readGraph("data/exemple1_from_chatGPT.txt");
    displayAdjaList(&list3);
    printf("\n");
    verifMarkov(&list3);
    printf("\n");

    //Essai la conversion des données du graph en données mermaid avec exemple_valid_step3.txt

    printf("Creation du fichier mermaid sur exemple_valid_step3.txt !\n");
    adj_list list4 = readGraph("data/exemple_valid_step3.txt");
    convertMermaid(&list4, "resultatmermaid.mmd");

    printf("==============================================\n");
    printf("        TEST PARTIE 2 : TARJAN + HASSE        \n");
    printf("==============================================\n\n");

    // =======================================================
    // 1) Charger un graphe de test
    // =======================================================
    printf("[1] Chargement du graphe exemple_valid_step3.txt...\n");
    adj_list g = readGraph("data/exemple_valid_step3.txt");
    printf("Graphe charge avec %d sommets.\n\n", g.size);

    printf("Liste d'adjacence :\n");
    displayAdjaList(&g);
    printf("\n");


    // =======================================================
    // 2) Exécuter Tarjan
    // =======================================================
    printf("[2] Execution de Tarjan...\n");

    tarjan_partition part = tarjan(&g);

    printf("Nombre de classes trouvees : %d\n\n", part.size);

    printf("Liste des classes detectees :\n");
    printPartition(&part);
    printf("\n");


    // =======================================================
    // 3) Calcul du tableau sommet -> classe
    // =======================================================
    printf("[3] Construction du tableau sommet -> classe...\n");

    int *classOf = computeVertexToClassTable(&part, &g);

    printf("Sommet | Classe\n");
    printf("----------------\n");

    for (int i = 0; i < g.size; i++)
        printf("  %2d   |   C%d\n", i + 1, classOf[i] + 1);

    printf("\n");


    // =======================================================
    // 4) Construire les liens entre classes
    // =======================================================
    printf("[4] Construction des liens entre classes...\n");

    t_link_array links = buildClassLinks(&g, &part);

    printf("Liens trouves (avant suppression transitifs) :\n");
    for (int i = 0; i < links.log_size; i++) {
        printf("  %s -> %s\n",
            part.classes[links.links[i].from].name,
            part.classes[links.links[i].to].name);
    }
    printf("\n");


    // =======================================================
    // 5) Supprimer les liens transitifs
    // =======================================================
    printf("[5] Suppression des liens transitifs...\n");

    removeTransitiveLinks(&links);

    printf("Liens finaux (diagramme de Hasse) :\n");
    for (int i = 0; i < links.log_size; i++) {
        printf("  %s -> %s\n",
            part.classes[links.links[i].from].name,
            part.classes[links.links[i].to].name);
    }
    printf("\n");


    // =======================================================
    // 6) Export du Hasse en Mermaid
    // =======================================================
    printf("[6] Export du diagramme de Hasse (hasse_test_output.mmd)...\n");

    convertHasseToMermaid(&part, &links, "hasse_test_output.mmd");

    printf("Fichier genere : hasse_test_output.mmd\n");
    printf("\n");

    // =======================================================
    // 7) Affichage des caractéristiques
    // =======================================================

    printf("[7] Etape 3 : caracteristiques du graphe\n\n");

    displayGraphSpecificity(&part, &links);


    // *************************
    // Partie 3 : matrices / distributions
    // *************************

    printf("==============================================\n");
    printf("        TEST PARTIE 3 : MATRICES / DISTRIBUTIONS\n");
    printf("==============================================\n\n");

    // -----------------------------------------------
    // 1) Chargement du graphe meteo + matrice M
    // -----------------------------------------------
    printf("[1] Chargement du graphe meteo (exemple_meteo.txt)...\n");
    adj_list g_meteo = readGraph("data/exemple_meteo.txt");
    printf("Graphe meteo charge avec %d etats.\n\n", g_meteo.size);

    printf("Liste d'adjacence du graphe meteo :\n");
    displayAdjaList(&g_meteo);
    printf("\n");

    printf("[2] Construction de la matrice de transition M...\n");
    t_matrix M = createMatrixFromAdjList(&g_meteo);

    printf("Matrice M (transition d'un jour au suivant) :\n");
    printMatrix(M);
    printf("\n");

    // -----------------------------------------------
    // 2) Calcul de M^3
    // -----------------------------------------------
    printf("[3] Calcul de M^3...\n");
    t_matrix Mpow = createZeroMatrix(M.n);
    t_matrix tmp  = createZeroMatrix(M.n);

    // Mpow = M
    copyMatrix(Mpow, M);
    int power = 1;

    while (power < 3) {
        // tmp = Mpow * M
        multiplyMatrices(Mpow, M, tmp);
        // Mpow = tmp
        copyMatrix(Mpow, tmp);
        power++;
    }

    printf("M^3 :\n");
    printMatrix(Mpow);
    printf("\n");

    // -----------------------------------------------
    // 3) Calcul de M^7
    // -----------------------------------------------
    printf("[4] Calcul de M^7...\n");

    // On repart de Mpow = M
    copyMatrix(Mpow, M);
    power = 1;

    while (power < 7) {
        multiplyMatrices(Mpow, M, tmp);  // tmp = Mpow * M
        copyMatrix(Mpow, tmp);           // Mpow = tmp
        power++;
    }

    printf("M^7 :\n");
    printMatrix(Mpow);
    printf("\n");

    // -----------------------------------------------
    // 4) Recherche d'un n tel que diff(M^n, M^(n-1)) < 0.01
    // -----------------------------------------------
    printf("[5] Recherche de n tel que diff(M^n, M^(n-1)) < 0.01...\n");

    // On repart encore de M^1 = M
    copyMatrix(Mpow, M);
    power = 1;
    double eps = 0.01;

    while (1) {
        // tmp = M^(power+1)
        multiplyMatrices(Mpow, M, tmp);

        double d = diffMatrix(tmp, Mpow);
        printf("diff(M^%d, M^%d) = %.6f\n", power + 1, power, d);

        copyMatrix(Mpow, tmp);
        power++;

        if (d < eps || power > 100) {
            break;  // on s'arrete soit parce que ça converge, soit par sécurité
        }
    }

    printf("\nOn s'arrete a n = %d (diff < %.2f ou max iterations atteint).\n", power, eps);
    printf("M^%d :\n", power);
    printMatrix(Mpow);
    printf("\n");



    free(classOf);
    return 0;
}
