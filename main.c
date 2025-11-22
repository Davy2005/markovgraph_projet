#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"
#include "utils.h"


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

    free(classOf);
    return 0;
}
