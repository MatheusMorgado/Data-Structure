#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
static int *pre ,*low;
static int *stack;
static int cnt;
static int k,n;

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
        printf("%d:\n", p->id_no);
        TViz *v = p->prim_viz;
        while (v) {
            printf("viz: %d \t  custo:%d\n", v->id_viz, v->custo);
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
    TNO *p = g->prim, *ant = NULL;

    while ((p) && p->id_no != id) {
        ant = p;
        p = p->prox_no;
    }

    if (!p) return;

    TViz *v = p->prim_viz;
    while (v) {
        retira_aresta(g, id, v->id_viz);
        v = p->prim_viz;
    }

    if (!ant) {
        g->prim = p->prox_no;
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
    int oriented = 1;
    for (int j = 0; j < total_nodes; j++) {
        if (values[j]) {
            oriented = 0;
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
    while (p->id_no != check) {
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
    reachR(g, 1);
    for (int j = 1; j <= total; ++j) {
        if (visit[j] == 0) {
            return 0;
        }
    }
    return 1;
}

int checkValidBridge(TG *g, int from, int to, int total) {
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
        aux->prox_viz = p->prox_viz;
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
    int i = 0;
    while (p) {
        TViz *viz = copyViz(p->prim_viz);
        while (viz) {
            int id_no = p->id_no;
            int id_viz = viz->id_viz;
            if (checkValidBridge(g, id_no, id_viz, total)) {
                printf("bridge %d: %d -> %d\n", i, id_no, id_viz);
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
    if (tested == 1) {
        reachArticulation(g, 2, tested);
    } else {
        reachArticulation(g, 1, tested);
    }
    for (int j = 1; j <= total; j++) {
        if (visit[j] == 0 && j != tested) {
            return 0;
        }
    }
    return 1;
}

void printArticulations(TG *g, int total) {
    for (int i = 1; i <= total; i++) {
        if (!graphStillConnectedForArticulation(g, total, i)) {
            printf("articulation point: %d\n", i);
        }
    }
}


// gamb

static void reachClusters(TG *G, int check) {
    printf(" %d", check);
    visit[check] = 1;
    TNO* p = G -> prim;
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
    for (int j = 0; j < total; ++j) {
        if (!visit[j]) {
            printf("cluster %d:", count++);
            reachClusters(g, j);
            printf("\n");
        }
    }
}


int Graphsct(TG* grafo , int sc[],int totalVertices){

    int v;
    pre = malloc(totalVertices*sizeof(int));
    low = malloc(totalVertices*sizeof(int));
    stack = malloc(totalVertices*sizeof(int));

    for(v = 0 ; v < totalVertices ; v++ ){
        pre[v] = sc[v] = -1;
    }
    cnt = k = n = 0;

    for (v = 0 ; v < totalVertices ;v++)
    {
        if(pre[v] == -1){
            pre[v] = v;
            strongR(grafo,v,sc);
        }
    }
    free(pre);free(low);free(stack);
    return k;
}

void strongR(TG* grafo,int v,int sc[]){
    int w ,u,min;
    TViz* a;
    pre[v] = cnt++;
    min = pre[v];
    stack[n++] = v;
    TNO* no = busca_no(grafo,v);
    for(a = no-> prim_viz; a ; a =a-> prox_viz){
        w = a->id_viz;
        if(pre[w] == -1){
            strongR(grafo,w,sc);
            if(low[w] < min) min = low[w];
        }
        else if(pre[w] < pre[v] && sc[w] == -1){
            if(pre[w] < min )min = pre[w];
        }
    }
    low[v] = min;
    if(low[v] == pre[v]){
        do{
            u = stack[--n];
            sc[u] = k;
        }while(u != v);
        k++;
    }
}

void show_strong_components(TG* grafo,int sc[],int totaldeVertices){
    int iterator,gambiarra;
    gambiarra = Graphsct(grafo,sc,totaldeVertices);
    for(iterator = 0;iterator < gambiarra ;iterator++){
        for(int dh = 0 ;dh < totaldeVertices ; dh++){
            if(sc[dh] == iterator) printf("%d ",dh);
        }
        printf("\n");
    }
}