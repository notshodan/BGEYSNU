#include "vectors.h"

void fillVect(vector* v, cell* g0, int* n){
    v->c[*n] = g0;
    *n += 1;
    for(int i = 0; i < g0->type; i++){  
        if(transition_rules(*g0, i) > 0 && g0->move[i] != NULL){
            fillVect(v, g0->move[i], n);
        }
    }
}

void freeVect(vector* v){
    free(v->c);
    free(v);
}

vector* initVector(int n, cell* g0){
    vector* v = malloc(sizeof(vector));
    cell** t = malloc(sizeof(cell*) * n);
    v->n = n;
    v->c = t;
    int a = 0;
    fillVect(v, g0, &a);
    return v;
}

int TileCount(cell* g0){
    int res = 1;
    for(int i = 0; i < g0->type; i++){
        if(g0->move[i] != NULL && transition_rules(*g0, i) > 0){
            res += TileCount(g0->move[i]); 
        }
    }
    return res;
}
