#include <stdio.h>
#include "utils.h"

int main() {

    // Partie 1

    //Teste sur les donnés de exemple1.txt
    adjlist test = readGraph("data/exemple1.txt");
    printAdjList(&test);

    printf("\n");
    verifGraphDeMarkov(&test);
    printf("\n");

    //Teste sur les donnés de exemple3.txt
    adjlist test2 = readGraph("data/exemple3.txt");
    printAdjList(&test2);

    printf("\n");
    verifGraphDeMarkov(&test2);
    printf("\n");

    //Teste sur les donnés de exemple1_from_chatGPT.txt
    adjlist test3 = readGraph("data/exemple1_from_chatGPT.txt");
    printAdjList(&test3);

    printf("\n");
    verifGraphDeMarkov(&test3);
    printf("\n");

    //Test sur l'exportation du graph avec exemple_valid_step3.txt


    adjlist test4 = readGraph("data/exemple_valid_step3.txt");
    creerMermaid(&test4, "resultatgraph.mmd"); // Génère un fichier MMD comptabile pour l'affichage sur Mermaid.


    return 0;
}
