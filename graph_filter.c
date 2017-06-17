#include <stdio.h>
#include <stdlib.h>
#include "graph.c"


int main() {
    FILE *arq = fopen("trab1/13.txt", "r");
    if (!arq) {
        exit(1);
    }

    // lendo total de nós
    int total_nodes;
    fscanf(arq, "%d", &total_nodes);
    printf("%d\n", total_nodes);


    TG *graph = cria();
    // inserindo nós
    for (int i = total_nodes; i >= 0; i--) {
        insere_no(graph, i + 1);
    }

    // inserindo arestas

    int *aux = malloc(sizeof(int) * total_nodes);
    int from, to;
    while (fscanf(arq, "%d %d", &from, &to) != EOF) {
        aux[from - 1] += to;
        aux[to - 1] -= from;
        insere_aresta(graph, from, to, 1);
        printf("%d %d\n", from, to);
    }

    int oriented = checkOrientation(aux, total_nodes);

    printBridges(graph,total_nodes);
    int sc[1000] = {-1};
    printaCFC(graph,sc,total_nodes);


    fclose(arq);
    free(aux);

    return 0;
}
