#include <stdio.h>
#include <stdlib.h>
#include "graph.c"


int main() {
    int fnSize = 50;
    printf("Digite nome do arquivo:(max 50 caracteres)\n");
    char *fname = malloc(sizeof(char) * fnSize);
    scanf("%s", fname);

    FILE *arq = fopen(fname, "r");
    if (!arq) {
        exit(1);
    }

    // lendo total de nós
    int total_nodes;
    fscanf(arq, "%d", &total_nodes);
    printf("%d\n", total_nodes);


    TG *graph = cria();
    // inserindo nós
    for (int i = total_nodes -1; i >= 0; i--) {
        insere_no(graph, i);
    }

    // inserindo arestas

    int *aux = malloc(sizeof(int) * total_nodes);
    int from, to;
    while (fscanf(arq, "%d %d", &from, &to) != EOF) {
        aux[from - 1] += to;
        aux[to - 1] -= from;
        insere_aresta(graph, from-1, to-1, 1);
    }

    fclose(arq);

    int option = 1;
    while (option) {
        int sc[1000];
        imprime(graph);
        int oriented = checkOrientation(aux, total_nodes);
        int connected = graphStillConnected(graph, total_nodes);

        if (oriented) {
            show_strong_components(graph, sc, total_nodes);
        } else {
            if (connected) {
                printBridges(graph, total_nodes);
                printArticulations(graph, total_nodes);
            } else {
                printClusters(graph, total_nodes);
            }
        }


        printf("\nDigite o que quer fazer agora:\n");
        printf("1-inserir aresta\n2-retirar aresta\n3-buscar aresta\n");
        printf("4-inserir vertice\n5-retirar vertice\n6-buscar vertice\n");
        printf("Digite qualquer outro numero pra terminar o programa\n");
        scanf("%d", &option);
        int x1, x2;
        switch (option) {

            case 1:
                printf("Digite a aresta a ser inserida:\n");
                scanf("%d %d", &x1, &x2);
                aux[x1 - 1] += x2;
                aux[x2 - 1] -= x1;
                insere_aresta(graph, x1-1, x2-1, 1);
                break;
            case 2:
                printf("Digite a aresta a ser removida\n");
                scanf("%d %d", &x1, &x2);
                aux[x1 - 1] -= x2;
                aux[x2 - 1] += x1;
                retira_aresta(graph, x1-1, x2-1);
                break;
            case 3:
                printf("Digite a aresta a ser buscada:\n");
                scanf("%d %d", &x1, &x2);
                busca_aresta(graph, x1-1, x2-1);
                break;
            case 4:
                printf("Digite o vertice a ser inserido:\n");
                scanf("%d", &x1);
                insere_no(graph, x1-1);
                total_nodes++;
                break;
            case 5:
                printf("Digite o vertice a ser removido\n");
                scanf("%d", &x1);
                retira_no(graph, x1-1);
                total_nodes = total_nodes-1;
                break;
            case 6:
                printf("Digite o vertice a ser buscado:\n");
                scanf("%d", &x1);
                busca_no(graph, x1-1);
                break;
            case 7:
                break;
            default:
                option = 0;
                break;
        }

    }

    free(aux);


    return 0;
}