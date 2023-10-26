// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET


// ---------- TESSELATION DRAFT ---------- //


// for now, we only work with regular tilings, ie each tesselation has only one type of tile.



typedef struct cell_s {
    int type;                // number of neighbours
    int state;
    struct cell_s** move;    // list of neighbours
    int* spin;               // connections -- these last two fields mean that (g,i) connects to (g.move[i], g.spin[i])
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

void incr_walker(walker* w, signed int k){
    w->edge = (w->edge + k) % ((w->c)->type);
}

walker* init_walker(cell* c, int index){
    walker* w = malloc(sizeof(walker));
    w->c = c;
    w->edge = index;
    return w;
}

void step_walker(walker* w){
    int i = w->c->type;
    
    w->cell = w->c->move[i];
    // ... case if c = NULL !
    w->edge = w->c->spin[i];
}

cell* NewTile(int type, int state){
    cell* n = malloc(sizeof(cell));
    n->type = type;
    n->state = state;
    n->move = malloc(sizeof(cell*) * type);
    for(int i = 0; i < type; i++){
        n->move[i] = NULL;
    }
    n->spin = calloc(sizeof(int), type);

    return n;
}

void free_tile(cell* c){
    free(c->move);
    free(c->spin);
    free(c);
}

void Generate_RTS(cell* c, int range){
    for(int i = 0; i < c->type, i++){
        if(c->move[i] == NULL){
            int x = transition_rules(*c, i);
            if(x > 0){
                cell* v = NewTile(c->type, x, c->dist + 1);

                v->spin[0] = i;
                v->move[0] = c;
                c->spin[i] = x;
                c->move[i] = v;

                if(range > 0){
                    Generate_RTS(v, tab, range - 1);
                }  
            }
            if(x == -1){
                walker* w = init_walker(c, i);
                incr_walker(w, -1);

                while(transition_rules(*(w->cell), w->edge) != -2){
                    step_walker(w);
                    incr_walker(w, -1);    
                }

                cell* v = w->cell;
                int j = w->edge;
                
                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);
            }
            if(x == -2){
                walker* w = init_walker(c, i);
                incr_walker(w, 1);

                while(transition_rules(*(w->cell), w->edge) != -1){
                    step_walker(w);
                    incr_walker(w, 1);
                }

                cell* v = w->cell;
                int j = w->edge;

                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);
            }
        }
    }
}