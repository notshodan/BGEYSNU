// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

// for now, we only work with regular tilings, ie each tesselation has only one type of tile.

#include "tesselation.h"

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
    /*
    This function only implements Pre-computed GRTS' for {4,5}, {5,4} and {7,3} regular tesselations
    */
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

cell* NewTile(int type, int state){
    /*
    Generates a new tile, with all neighbors set to NULL
    */
    cell* n = malloc(sizeof(cell));
    assert(n != NULL);

    n->type = type;
    n->state = state;

    n->move = malloc(sizeof(cell*) * type);
    assert(n->move != NULL);

    for(int i = 0; i < type; i++){
        n->move[i] = NULL;
    }

    n->spin = calloc(sizeof(int), type);

    return n;
}

void incr_walker(walker* w, int k){
    /*
    Rotates a walker k times (clockwise if k > 0, else counterclockwise)
    */
    w->edge = (w->edge + k) % ((w->tile)->type);
    while(w->edge < 0){
        w->edge += w->tile->type;
    }
}

walker* init_walker(cell* c, int index){
    /*
    Initializes a walker looking at the i-th edge of cell c
    */
    walker* w = malloc(sizeof(walker));
    assert(w != NULL);

    w->tile = c;
    w->edge = index;
    return w;
}

void step_walker(walker* w){
    /*
    Steps the walker forward, that is, if w = (t, i), w + STEP := (w->t->move[i], w->t->spin[i])
    */
    cell* t = w->tile;
    int i = w->edge;

    if(t->move[i] == NULL){
        /*
        Case 1 : No tile beyond i-th edge
        */
        int s = transition_rules(*t, i);

        if(s > 0){
            /*
            If s is positive then i is a tree edge connecting t to a child t' in state s
            */
            t->move[i] = NewTile(t->type, transition_rules(*t, i));
            t->spin[i] = 0;

            t->move[i]->move[0] = w->tile;
            t->move[i]->spin[0] = w->edge;

            w->tile = t->move[i];
            w->edge = 0;
        }
        else{
            /*
            Else we recursively call Generate_RTS to create all the cells in a 1-tile radius
            so the walker can move forward
            */
            Generate_RTS(t, 0);
            assert(t->move[i] != NULL);
            w->tile = t->move[i];
            w->edge = t->spin[i];     
        }
    }
    else{
        /*
        Case 2 : A tile beyond i-th edge already exists
        */
        w->tile = t->move[i];
        w->edge = t->spin[i];
    }
}


// Lazy solutions to dumb problems : a bruning passion
void connect7_1(walker* w){
    incr_walker(w, -1);
    step_walker(w);
    incr_walker(w, -1);
    incr_walker(w, -1);
    step_walker(w);
    incr_walker(w, -1);
    step_walker(w);
    incr_walker(w, -1);
    incr_walker(w, -1);
}

void connect71(walker* w){
    incr_walker(w, 1);
    incr_walker(w, 1);
    step_walker(w);
    incr_walker(w, 1);
    step_walker(w);
    incr_walker(w, 1);
    incr_walker(w, 1);
    step_walker(w);
    incr_walker(w, 1);
}


/**
 * \fn Generate_RTS(cell* c, int range)
 *  \brief Generates depth-first the combinatorial map of a tesselation in a radius around a starting point
 *
 *  \param c The cell which serves as the starting point
 *  \param range The radius of generation around c.
 *
 *  \return Void, and modifies directly the values of c.
 */
void Generate_RTS(cell* c, int range){
    //sleep(1);
    for(int i = 0; i < c->type; i++){
        if(c->move[i] == NULL){
            int x = transition_rules(*c, i);
            if(x > 0){
                cell* v = NewTile(c->type, x);

                v->spin[0] = i;
                v->move[0] = c;
                c->spin[i] = 0;
                c->move[i] = v;

                // printf("Now, current cell is connected to a cell in state %d.\n\n", v->state);
                if(range > 0){
                    Generate_RTS(v, range - 1);
                }  
            }
            if(x == -1){
                // printf("Current cell is to the left of a non-tree edge. Walking around it.\n");
                walker* w = init_walker(c, i);
                if(c->type == 7 && c->state == 2 && i == 1){
                    connect7_1(w);
                } else {
                    incr_walker(w, -1);

                    while(transition_rules(*(w->tile), w->edge) != -2){
                        step_walker(w);
                        incr_walker(w, -1);    
                    }
                }

                cell* v = w->tile;
                int j = w->edge;
                

                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);
            }
            if(x == -2){
                // printf("Current cell is to the right of a non-tree edge. Walking around it.\n");
                walker* w = init_walker(c, i);

                if((c->type == 7 && c->state == 2 && i == 5) || (c->type == 7 && c->state == 1 && i == 5)){
                    connect71(w);
                } else {
                    incr_walker(w, 1);

                    while(transition_rules(*(w->tile), w->edge) != -1){
                        step_walker(w);
                        incr_walker(w, 1);
                    }
                }

                cell* v = w->tile;
                int j = w->edge;

                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);
            }
            if(x == 0){ // PROBLEMS HERE !! 
                // printf("\nParent connection : /!\\ CONSTRUCTION ERROR /!\\\n\n");
                exit(1);
            }
        }
        else{
            if(range > 0){
                if(transition_rules(*c, i) > 0){
                    Generate_RTS(c->move[i], range - 1);
                }
                else{
                }
            }
            else{
                // printf("Next tile out of bounds.\n\n");
            }
        }
    }
}

void free_tile(cell* c){
    for(int i = 0; i < c->type; i++){
        if(transition_rules(*c, i) >  0 && c->move[i] != NULL){
            free_tile(c->move[i]);
        }
    }
    free(c->move);
    free(c->spin);
    free(c);
}

void print_RTS(cell* c, int level){
    if(c == NULL){
        return;
    }
    for(int i = 0; i < level; i++){
        printf(i == level - 1 ? "  |-" : "  | ");
    }
    printf("[ %d ]\n", c->state);
    for(int i = 0; i < c->type; i++){
        if(transition_rules(*c, i) > 0){
            print_RTS(c->move[i], level + 1);
        }
    }
}

