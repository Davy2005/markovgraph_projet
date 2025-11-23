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

    //Essai sur les donnés de exemple3.txt

    printf("Essai sur exemple3.txt :\n");
    adj_list list3 = readGraph("data/exemple3.txt");
    displayAdjaList(&list3);
    printf("\n");
    verifMarkov(&list3);
    printf("\n");

    //Essai la conversion des données du graph en données mermaid avec exemple_valid_step3.txt

    printf("Essai sur exemple_valid_step3.txt :\n");
    adj_list list4 = readGraph("data/exemple_valid_step3.txt");
    displayAdjaList(&list4);
    printf("\n");
    verifMarkov(&list4);
    printf("Creation du fichier mermaid sur exemple_valid_step3.txt !\n");
    convertMermaid(&list4, "exemple_valid_step3.mmd");

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
    printf("[6] Export du diagramme de Hasse (hasse_exemple_valid_step3.mmd)...\n");

    convertHasseToMermaid(&part, &links, "hasse_exemple_valid_step3.mmd");

    printf("Fichier genere : hasse_exemple_valid_step3.mmd\n");
    printf("\n");

    // =======================================================
    // 7) Affichage des caractéristiques
    // =======================================================

    printf("[7] Etape 3 : caracteristiques du graphe\n\n");

    displayGraphSpecificity(&part, &links);

    // =======================================================
    // PARTIE 3 - ETAPE 1 : calculs matriciels (exemple meteo)
    // =======================================================

    printf("==============================================\n");
    printf("        PARTIE 3 - ETAPE 1 : MATRICES\n");
    printf("==============================================\n\n");

    // 1) Graphe meteo -> matrice M
    printf("[1] Chargement du graphe meteo (exemple_meteo.txt)...\n");
    adj_list g_meteo = readGraph("data/exemple_meteo.txt");
    printf("Graphe meteo charge avec %d etats.\n\n", g_meteo.size);

    printf("Matrice de transition M :\n");
    t_matrix M = createMatrixFromAdjList(&g_meteo);
    printMatrix(M);
    printf("\n");

    // 2) Calcul de M^3
    printf("[2] Calcul de M^3...\n");
    t_matrix P = createZeroMatrix(M.n);
    t_matrix tmp = createZeroMatrix(M.n);

    copyMatrix(P, M);      // P = M^1
    int power = 1;
    while (power < 3) {
        multiplyMatrices(P, M, tmp);  // tmp = P * M
        copyMatrix(P, tmp);           // P = tmp
        power++;
    }
    printf("M^3 :\n");
    printMatrix(P);
    printf("\n");

    // 3) Calcul de M^7
    printf("[3] Calcul de M^7...\n");
    copyMatrix(P, M);      // repart de M^1
    power = 1;
    while (power < 7) {
        multiplyMatrices(P, M, tmp);
        copyMatrix(P, tmp);
        power++;
    }
    printf("M^7 :\n");
    printMatrix(P);
    printf("\n");

    // 4) Recherche d'un n tel que diff(M^n, M^(n-1)) < 0.01
    printf("[4] Recherche de n tel que diff(M^n, M^(n-1)) < 0.01...\n");
    double eps = 0.01;
    copyMatrix(P, M);  // P = M^1
    power = 1;

    while (1) {
        multiplyMatrices(P, M, tmp);       // tmp = M^(power+1)
        double d = diffMatrix(tmp, P);     // diff entre M^(n) et M^(n-1)
        printf("  diff(M^%d, M^%d) = %.6f\n", power + 1, power, d);

        copyMatrix(P, tmp);                // P = M^(power+1)
        power++;

        if (d < eps || power > 100) break; // sécurité si ça ne converge pas
    }

    printf("\nOn s'arrete a n = %d (diff < %.2f ou max iterations).\n", power, eps);
    printf("M^%d :\n", power);
    printMatrix(P);
    printf("\n");

    printf("Exportation des donnes de graph de meteo dans un fichier meteo_mermaid.mmd .\n");
    convertMermaid(&g_meteo, "meteo_mermaid.mmd");

    printf("\n");

    // =======================================================
    // PARTIE 3 - ETAPE 2 : distributions stationnaires par classe
    // (sur le graphe g = exemple_valid_step3.txt utilise en partie 2)
    // =======================================================

    printf("==============================================\n");
    printf("   PARTIE 3 - ETAPE 2 : distributions stationnaires par classe\n");
    printf("==============================================\n\n");

    // Matrice globale pour le graphe g (exemple_valid_step3.txt)
    t_matrix Mglob = createMatrixFromAdjList(&g);

    for (int c = 0; c < part.size; c++) {
        tarjan_class *C = &part.classes[c];

        printf("Classe %s :\n", C->name);

        // Sous-matrice correspondant à cette classe
        t_matrix Mc = subMatrix(Mglob, part, c);

        printf("  Sous-matrice Mc :\n");
        printMatrix(Mc);
        printf("\n");

        // Calcul des puissances de Mc jusqu'à stabilisation
        t_matrix Pc   = createZeroMatrix(Mc.n);  // contiendra Mc^n
        t_matrix tmpc = createZeroMatrix(Mc.n);

        copyMatrix(Pc, Mc);   // Pc = Mc^1
        int pwr = 1;

        while (1) {
            multiplyMatrices(Pc, Mc, tmpc);   // tmpc = Pc * Mc = Mc^(pwr+1)
            double d = diffMatrix(tmpc, Pc);  // diff entre Mc^n et Mc^(n-1)

            copyMatrix(Pc, tmpc);             // Pc = Mc^(pwr+1)
            pwr++;

            if (d < eps || pwr > 100) {
                printf("  -> Stabilisation (ou max iters) a n = %d, diff = %.6f\n",
                       pwr, d);
                break;
            }
        }

        // La premiere ligne de Pc est la distribution limite approx.
        printf("  Distribution limite approx pour la classe %s :\n", C->name);
        double somme = 0.0;
        for (int j = 0; j < Pc.n; j++) {
            double val = Pc.m[0][j];
            somme += val;
            int sommet = C->vertices[j];  // numero de sommet global
            printf("    Etat %d : %.4f\n", sommet, val);
        }
        printf("    Somme des probabilites = %.4f\n\n", somme);

    }
    
    free(classOf);
    return 0;
}
