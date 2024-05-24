// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "generation.h"

bool checkConstraint(cell* c, cell* g0){
    for(int i = 0; i < c->type; i++){
        if(c->move[i] == g0){
            return true;
        }
    }
    return false;
}

void setTileContent(vector* v, cell* g0){
    float DENSITY = 0.18;
    int TILE_NB = v->n;
    int BOMB_NB = (int) (TILE_NB * DENSITY);
    printf("Tile number : %d\nBomb number : %d\n", TILE_NB, BOMB_NB);
    int i = 0;
    while(i < BOMB_NB){
        int j = rand() % v->n;
        j = MAX(j, -j);
        if(v->c[j]->content != -1 && !checkConstraint(v->c[j], g0)){
            v->c[j]->content = -1;
            i++;
        }
    }
    for(int i = 0; i < v->n; i++){
        setTileInfo(v->c[i]);
    }
}

int coutDiagonals();

void setTileInfo(cell* c){
    if(c->content == -1){
        return;
    }
    int count = 0;
    cell* v;
    for(int i = 0; i < c->type; i++){
        v = c->move[i];
        if(v != NULL){
            if(c->move[i]->content == -1){
                count++;
            }
        }
    }
    c->content = count;
}