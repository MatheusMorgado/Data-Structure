#include <stdio.h>
#include <stdlib.h>

static int cnt0, pre[1000];
static int cnt1, post[1000];
static int vv[1000];

typedef struct viz {
    int id_viz;
    int custo;
    struct viz *prox_viz;
} TViz;

typedef struct no {
    int id_no;
    TViz *prim_viz;
    struct no *prox_no;
} TNO;

typedef struct grafo {
    TNO *prim;
} TG;

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


static int visit[1000] = {0};

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
    while(p) {
        TViz *aux = malloc(sizeof(TViz));
        aux->id_viz = p->id_viz;
        aux->custo = 1;
        aux->prox_viz = novo;
        novo = aux;
        p=p->prox_viz;
    }
    return novo;
}

void freeViz(TViz *viz){
    if(!viz){
        return;
    }

    while(viz){
        TViz *prox = viz->prox_viz;
        free(viz);
        viz=prox;
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



TG* REVSERSE(TG* g,int NUMEROTOTALDEVERTICES){
    int node_iterator,w;
    TViz* a;
    TG* gt = cria();
    TNO* no = busca_no(g,node_iterator);
    for(node_iterator = 0 ; node_iterator < NUMEROTOTALDEVERTICES ; node_iterator ++){
        insere_no(gt,node_iterator);
    }
    for(node_iterator = 0; node_iterator < NUMEROTOTALDEVERTICES ; node_iterator++){
        for(a =  no-> prim_viz;a;a = a-> prox_viz){
            w = a->id_viz;
            insere_aresta(gt,w,node_iterator,1);
        }
    }
    return gt;
}
void dfsR(TG* gt , int node_iterator){
    TViz* a;
    pre[node_iterator] = cnt0++;
    TNO* no = busca_no(gt,node_iterator);
    for(a =  no-> prim_viz ;a ; a = a-> prox_viz){
        if(pre[node_iterator] == -1){
            dfsR(gt,a ->id_viz);
        }
        post[node_iterator] = cnt1++;
    }
}

void dfsRSC(TG* g,int node_iterator,int* sc , int k){
    TViz* a ;
    sc[node_iterator] = k;
    TNO* no = busca_no(g,node_iterator);
    for(a = no-> prim_viz;a;a = a->prox_viz){
        if(a->id_viz == -1){
            dfsRSC(g,a->id_viz,sc,k);
        }
    }
}

void printaCFC(TG *g,int* sc,int NUMEROTOTALDEVERTICES){
    int z = graphsck(g,sc,NUMEROTOTALDEVERTICES);
    int i,k;
    for(i=0; i < z ; i++){
        for(k = 0 ; k < 1000 ; k ++)

            if(sc[k] == i ) printf("%d ", sc[i]);
    }
    printf("\n");
}

int graphsck(TG* g,int *sc,int NUMEROTOTALDEVERTICES){
    int k,i;
    int node_iterator;
    TG* gt = REVSERSE(g,NUMEROTOTALDEVERTICES);
    cnt0 = cnt1 = 0;
    for(node_iterator = 0 ; node_iterator < NUMEROTOTALDEVERTICES ; node_iterator++){
        pre[node_iterator] = -1;

    }
    for (node_iterator = 0 ; node_iterator < NUMEROTOTALDEVERTICES ; node_iterator++){
        if(pre[node_iterator] == -1){
            dfsR(gt,node_iterator);
        }
    }
    for (node_iterator = 0 ; node_iterator < NUMEROTOTALDEVERTICES ; node_iterator++){
        vv[post[node_iterator]] = node_iterator;
    }

    for (k = 0;i = NUMEROTOTALDEVERTICES-1,i >= 0 ; --i){
        node_iterator = vv[i];
        if(sc[node_iterator] == -1){
            dfsRSC(g,node_iterator,sc,k);
            k++;
        }
    }
    libera(gt);
    return k;
}