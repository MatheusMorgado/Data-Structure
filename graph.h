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

TG *cria(void);

TNO *busca_no(TG *g, int n);

void insere_no(TG *g, int no);

TViz *busca_aresta(TG *g, int no1, int no2);

void insere_aresta(TG *g, int id1, int id2, int custo);


void imprime(TG *g);


void retira_aresta(TG *g, int id1, int id2);

void retira_no(TG *g, int id);


void libera(TG *g);

// gamb

int checkOrientation(TG *g);