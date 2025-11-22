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
    printf("        TEST PARTIE 1 : GRAPH MARKOV + FICHIER MERMAID \n");
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

    //Conversion des données du graph en données mermaid avec exemple_valid_step3.txt

    printf("Creation du fichier mermaid sur exemple_valid_step3.txt !\n");
    adj_list list4 = readGraph("data/exemple_valid_step3.txt");
    convertMermaid(&list4, "resultatmermaid.txt");

    return 0;
}
