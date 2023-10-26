// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET


// ---------- TESSELATION DRAFT ---------- //


// for now, we only work with regular tilings, ie each tesselation has only one type of tile.



typedef struct cell_s {
    int type;                // number of neighbours
    int state;
    struct cell_s** move;    // list of neighbours
    int* spin;               // connections -- these last two fields mean that (g,i) connects to (g.move[i], g.spin[i])
    int dist;
} cell;

typedef struct walker_s {
    cell* c;
    int edge;
} walker;

int GRTS_5_4[3][5] = {
    {1, 1, 1, 1, 1},
    {0, 2, 1, 1, -2},
    {0, -1, 2, 1, -2}
};

int GRTS_4_5[7][4] = {
    {1, 1, 1, 1},
    {0, 3, 1, 4},
    {0, -1, 3, 4},
    {0, -1, 6, 4},
    {0, 3, 5, -2},
    {0, 3, 1, 2},
    {0, 2, 1, 4},
};
int GRTS_7_3[3][7] = {
    {1, 1, 1, 1, 1, 1, 1},
    {0, -1, 2, 1, 1, -2, -2},
    {0, -1, -1, 2, 1, -2, -2},
};

int transition_rules(cell c, int i){  // 0 : P ; -1 : L ; -2 : R ; q > 0 : Child
        switch(c.type){
            case 4:
                return GRTS_4_5[c.state][i];
                break;
            case 5:
                return GRTS_5_4[c.state][i];
                break;
            case 7:
                return GRTS_7_3[c.state][i];
                break;
            default:
                return -10;
                break;
        }
    return -10;
}

int valence(int type){
    switch(type){
        case 4:
            return 5;
            break;
        case 5:
            return 4;
            break;
        case 7:
            return 3;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

int card_Q(int type){
    switch(type){
        case 4:
            return 3;
            break;
        case 5:
            return 7;
            break;
        case 7:
            return 3;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

void incr_walker(walker* w, int k){
    w->edge = (w->edge + k) % ((w->c)->type);
}

walker init_walker(cell* c, int index){
    walker w = {
        c;
        index;
    };
    return w;
}

void step_walker(walker* w){
    int i = w->c->type;
    cell* c = w->c->move[i];
    int edge = w->c->spin[i];
    // ... case if c = NULL !
}

cell* NewTile(int type, int state, int dist){
    cell* n = malloc(sizeof(cell));
    n->type = type;
    n->state = state;
    n->move = malloc(sizeof(cell*) * type);
    for(int i = 0; i < type; i++){
        n->move[i] = NULL;
    }
    n->spin = calloc(sizeof(int), type);
    n->dist = dist;

    return n;
}

void free_tile(cell* c){
    free(c->move);
    free(c->spin);
    free(c);
}

void Generate_RTS(cell* c, cell** tab, int range){
    for(int i = 0; i < c->type, i++){
        /* 
        if((c->move[i] != NULL) && (c->spin[i] >= 0)){
            if(c->dist + 1 < tab[c->state]->dist){
                tab[c->state] = c->move[i];
                Generate_RTS(c->move(i), tab, range);
            }
            else {
                c->move[i] = tab[c->state];
            }
        }  RETARDED NONSENSE
        */
        if(c->move[i] == NULL){
            int x = transition_rules(*c, i);
            if(x > 0){
                cell* v = NewTile(c->type, x, c->dist + 1);
                v->spin[0] = i;
                v->move[0] = c;
                c->spin[i] = x;
                c->move[i] = v;
            }
            if(x == -1){
                walker w = init_walker(c, i);
                
            }
            if(x == -2){
                
            }
            if(x == 0){
            }
        }
    }
}