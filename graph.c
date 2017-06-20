#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

static int *pre, *low;
static int *stack;
static int cnt;
static int k, n;

TG *cria(void) {
    TG *g = (TG *) malloc(sizeof(TG));
    g->prim = NULL;
    return g;
}

TNO *busca_no(TG *g, int n) {
    TNO *p = g->prim;
    while ((p) && (p->id_no != n)) {
        p = p->prox_no;
    }
    return p;
}

void insere_no(TG *g, int no) {
    TNO *p = busca_no(g, no);
    if (p)return;
    p = (TNO *) malloc((sizeof(TNO)));
    p->id_no = no;
    p->prim_viz = NULL;
    p->prox_no = g->prim;
    g->prim = p;
}

TViz *busca_aresta(TG *g, int no1, int no2) {
    TNO *viz1 = busca_no(g, no1);
    if (!viz1)return NULL;
    TNO *viz2 = busca_no(g, no2);
    if (!viz2)return NULL;

    TViz *p = viz1->prim_viz;
    while ((p) && (p->id_viz != no2)) {
        p = p->prox_viz;
    }
    return p;
}

void insere_aresta(TG *g, int id1, int id2, int custo) {
    TNO *p1 = busca_no(g, id1);
    if (!p1)return;
    TNO *p2 = busca_no(g, id2);
    if (!p2)return;
    TViz *v = busca_aresta(g, id1, id2);
    if (v) {
        v->custo = custo;
        return;
    }
    v = (TViz *) malloc(sizeof(TViz));
    v->id_viz = id2;
    v->custo = custo;
    v->prox_viz = p1->prim_viz;
    p1->prim_viz = v;
}


void imprime(TG *g) {
    TNO *p = g->prim;
    while (p) {
        printf("%d:\n", p->id_no+1);
        TViz *v = p->prim_viz;
        while (v) {
            printf("viz: %d \t  custo:%d\n", v->id_viz+1, v->custo);
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
}


void retira_aresta(TG *g, int id1, int id2) {
    TNO *p1 = busca_no(g, id1);
    if (!p1)return;
    TNO *p2 = busca_no(g, id2);
    if (!p2)return;

    TViz *v = p1->prim_viz, *ant = NULL;
    while ((v) && (v->id_viz != id2)) {
        ant = v;
        v = v->prox_viz;
    }
    if (!v)return;;
    if (!ant)
        p1->prim_viz = v->prox_viz;
    else
        ant->prox_viz = v->prox_viz;

    free(v);
}

void retira_no(TG *g, int id) {
    TNO *p = g->prim, *ant = NULL ,*aux;
    TViz *v;
    while ((p) && p->id_no != id) {
        ant = p;
        v = p->prim_viz;
        while (v) {
            if(p->id_no == id){
                v = p->prim_viz;
                retira_aresta(g, id, v->id_viz);
                v = p->prim_viz;
            }

            else if(v->id_viz == id){
                retira_aresta(g,p->id_no,id);
            }
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
    aux = p->prox_no;
    while(aux){
        v = aux->prim_viz;
        while(v){
            if(v->id_viz == id)retira_aresta(g,aux->id_no,v->id_viz);
            v = v->prox_viz;
        }
        aux = aux->prox_no;
    }


    if (!ant) {
        g->prim = p-> prox_no;
    } else {
        ant->prox_no = p->prox_no;
    }
    free(p);

}


void libera(TG *g) {
    TNO *p = g->prim;
    while (p) {
        TViz *v = p->prim_viz;
        while (v) {
            TViz *t = v;
            v = v->prox_viz;
            free(t);
        }

        TNO *q = p;
        p = p->prox_no;
        free(q);
    }
    free(g);
}


static int visit[1000] = {0};


// gamb

int checkOrientation(int *values, int total_nodes) {
    int oriented = 0;
    for (int j = 0; j < total_nodes; j++) {
        if (values[j]) {
            oriented = 1;
            break;
        }
    }
    if (oriented) {
        printf("orientado\n");
    } else {
        printf("nao orientado\n");
    }
    return oriented;
}


static void reachR(TG *G, int check) {
    visit[check] = 1;
    TNO *p = G->prim;
    while ((p) && p->id_no != check) {
        p = p->prox_no;
    }
    TViz *viz = p->prim_viz;
    while (viz) {
        if (visit[viz->id_viz] == 0) {
            reachR(G, viz->id_viz);
        }
        viz = viz->prox_viz;
    }

}

int graphBFS(TG *G, int from, int searched) {
    for (int i = 0; i < 1000; ++i) {
        visit[i] = 0;
    }
    reachR(G, from);
    if (visit[searched] == 0) {
        return 0;
    } else {
        return 1;
    }
}

int graphStillConnected(TG *g, int total) {
    for (int i = 0; i < 1000; ++i) {
        visit[i] = 0;
    }
    TNO *p = g->prim;
    reachR(g, p->id_no);

    while(p){
        if(visit[p->id_no] == 0){
            return 0;
        }
        p = p->prox_no;
    }
    return 1;
}

int checkValidBridge(TG *g, int from, int to, int total) {
    if (from > to) {
        return 0;
    }
    retira_aresta(g, from, to);
    retira_aresta(g, to, from);

    int ans = !graphStillConnected(g, total);

    insere_aresta(g, from, to, 1);
    insere_aresta(g, to, from, 1);

    return ans;
}


// gamb

TViz *copyViz(TViz *viz) {
    if (!viz) {
        return;
    }
    TViz *novo = NULL;
    TViz *p = viz;
    while (p) {
        TViz *aux = malloc(sizeof(TViz));
        aux->id_viz = p->id_viz;
        aux->custo = 1;
        aux->prox_viz = novo;
        novo = aux;
        p = p->prox_viz;
    }
    return novo;
}

void freeViz(TViz *viz) {
    if (!viz) {
        return;
    }

    while (viz) {
        TViz *prox = viz->prox_viz;
        free(viz);
        viz = prox;
    }
}

void printBridges(TG *g, int total) {
    TNO *p = g->prim;
    int i = 1;
    while (p) {
        TViz *viz = copyViz(p->prim_viz);
        while (viz) {
            int id_no = p->id_no;
            int id_viz = viz->id_viz;
            if (checkValidBridge(g, id_no, id_viz, total)) {
                printf("bridge %d: %d -> %d\n", i, id_no + 1, id_viz + 1);
                i++;
            }
            viz = viz->prox_viz;
        }
        free(viz);

        p = p->prox_no;
    }
}


// gamb
static void reachArticulation(TG *G, int check, int tested) {
    if (check == tested) {
        return;
    }
    visit[check] = 1;
    TNO *p = G->prim;
    while (p->id_no != check) {
        p = p->prox_no;
    }
    TViz *viz = p->prim_viz;
    while (viz) {
        if (visit[viz->id_viz] == 0) {
            reachArticulation(G, viz->id_viz, tested);
        }
        viz = viz->prox_viz;
    }

}

int graphStillConnectedForArticulation(TG *g, int total, int tested) {
    for (int i = 0; i < 1000; ++i) {
        visit[i] = 0;
    }
    if (tested == 0) {
        reachArticulation(g, 1, tested);
    } else {
        reachArticulation(g, 0, tested);
    }

    TNO *p = g->prim;
    while(p){
        if(visit[p->id_no] == 0 && p->id_no != tested)return 0;
    }
    /*
    for (int j = 0; j < total; j++) {
        if (visit[j] == 0 && j != tested) {
            return 0;
        }
    }
     */
    return 1;
}

void printArticulations(TG *g, int total) {
    TNO *p = g->prim;
    while(p){
        if(!graphStillConnectedForArticulation(g,total,p->id_no))printf("articulation point: %d\n", p->id_no + 1);
        p = p->prox_no;
    }
    /*
    for (int i = 0; i < total; i++) {
        if (!graphStillConnectedForArticulation(g, total, i)) {
            printf("articulation point: %d\n", i + 1);
        }
    }
     */
}


// gamb

static void reachClusters(TG *G, int check) {
    printf(" %d", check + 1);
    visit[check] = 1;
    TNO *p = G->prim;
    while ((p) && p->id_no != check) {
        p = p->prox_no;
    }
    TViz *viz = p->prim_viz;
    while (viz) {
        if (visit[viz->id_viz] == 0) {
            reachClusters(G, viz->id_viz);
        }
        viz = viz->prox_viz;
    }

}


void printClusters(TG *g, int total) {
    for (int i = 0; i < 1000; ++i) {
        visit[i] = 0;
    }

    int count = 1;

    TNO *p = g->prim;
    while (p){
        if(!visit[p->id_no]){
            printf("cluster %d:", count++);
            reachClusters(g, p->id_no);
            printf("\n");
        }
        p=p->prox_no;
    }
    /*
    for (int j = 0; j < total; ++j) {
        if (!visit[j]) {
            printf("cluster %d:", count++);
            reachClusters(g, j);
            printf("\n");
        }
    }
     */
}


int Graphsct(TG *grafo, int sc[], int totalVertices) {

    int v;
    pre = malloc(totalVertices * sizeof(int));
    low = malloc(totalVertices * sizeof(int));
    stack = malloc(totalVertices * sizeof(int));


    TNO* p = grafo->prim;
    while(p){
        pre[p->id_no] = sc[p->id_no] = -1;
        p = p->prox_no;
    }
    cnt = k = n = 0;

    p = grafo->prim;

    while (p){
        if(pre[p->id_no] == -1){
            strongR(grafo,p->id_no,sc);
        }
        p = p -> prox_no;
    }

    free(pre);
    free(low);
    free(stack);
    return k;
}

void strongR(TG *grafo, int v, int sc[]) {
    int w, u, min;
    TViz *a;
    pre[v] = cnt++;
    min = pre[v];
    stack[n++] = v;
    TNO *no = busca_no(grafo, v);
    for (a = no->prim_viz; a; a = a->prox_viz) {
        w = a->id_viz;
        if (pre[w] == -1) {
            strongR(grafo, w, sc);
            if (low[w] < min) min = low[w];
        } else if (pre[w] < pre[v] && sc[w] == -1) {
            if (pre[w] < min)min = pre[w];
        }
    }
    low[v] = min;
    if (low[v] == pre[v]) {
        do {
            u = stack[--n];
            sc[u] = k;
        } while (u != v);
        k++;
    }
}

void show_strong_components(TG *grafo, int sc[], int totaldeVertices) {
    int iterator, gambiarra;
    TNO* no = grafo->prim;
    gambiarra = Graphsct(grafo, sc, totaldeVertices);
    for (iterator = 0; iterator < gambiarra; iterator++) {
        for (no = grafo->prim; no; no=no->prox_no) {
            if (sc[no->id_no] == iterator) printf("%d ", no->id_no+1);
        }
        printf("\n");
    }
    for(int i = 0; i < 1000 ; i ++){
        sc[i] = -1;
    }
}