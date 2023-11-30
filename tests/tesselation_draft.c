// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 minsieurmoi


// ---------- TESSELATION DRAFT ---------- //


// for now, we only work with regular tilings, ie each tesselation has only one type of tile.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

typedef struct cell_s {
    int type;                // number of neighbours
    int state;
    struct cell_s** move;    // list of neighbours
    int* spin;               // connections -- these last two fields mean that (g,i) connects to (g.move[i], g.spin[i])
} cell;

typedef struct walker_s {
    cell* tile;
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

void Generate_RTS(cell*, int);

void step_walker(walker* w){
    /*
    Steps the walker forward, that is, if w = (t, i), w + STEP := (w->t->move[i], w->t->spin[i])
    */
    cell* t = w->tile;
    int i = w->edge;

    printf("Walker w = (%d, %d)\n", t->state, i);

    if(t->move[i] == NULL){
        /*
        Case 1 : No tile beyond i-th edge
        */
        int s = transition_rules(*t, i);

        if(s > 0){
            /*
            If s is positive then i is a tree edge connecting t to a child t' in state s
            */
            printf("Creating a tile to step to...\n");
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
            printf("This AIN'T looking fresh, chief... Analyzing the surroundings.\n");
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
    printf("w + STEP = (%d, %d)\n", w->tile->state, w->edge);
}

void free_tile(cell*);

void Generate_RTS(cell* c, int range){
    printf("\nCurrently in state q = %d\n", c->state);
    for(int i = 0; i < c->type; i++){
        printf("Looking at edge index %d/%d\n", i, c->type - 1);
        if(c->move[i] == NULL){
            printf("No connected tile beyond its %d-th edge !\n", i);
            int x = transition_rules(*c, i);
            printf("GRTS[%d][%d] = %d\n", c->state, i, x);
            if(x > 0){
                printf("x = %d > 0 : child connection\n", x);
                cell* v = NewTile(c->type, x);

                v->spin[0] = i;
                v->move[0] = c;
                c->spin[i] = 0;
                c->move[i] = v;

                printf("Now, current cell is connected to a cell in state %d.\n\n", v->state);
                if(range > 0){
                    Generate_RTS(v, range - 1);
                }  
            }
            if(x == -1){
                printf("Current cell is to the left of a non-tree edge. Walking around it.\n");
                walker* w = init_walker(c, i);
                incr_walker(w, -1);

                while(transition_rules(*(w->tile), w->edge) != -2){
                    step_walker(w);
                    incr_walker(w, -1);    
                }

                cell* v = w->tile;
                int j = w->edge;
                
                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);

                printf("Now, current cell is connected to a cell in state %d.\n\n", v->state);
            }
            if(x == -2){
                printf("Current cell is to the right of a non-tree edge. Walking around it.\n");
                walker* w = init_walker(c, i);
                incr_walker(w, 1);

                while(transition_rules(*(w->tile), w->edge) != -1){
                    step_walker(w);
                    incr_walker(w, 1);
                }

                cell* v = w->tile;
                int j = w->edge;

                c->move[i] = v;
                c->spin[i] = j;
                v->move[j] = c;
                v->spin[j] = i;

                free(w);

                printf("Now, current cell is connected to a cell in state %d.\n\n", v->state);
            }
            if(x == 0){ // PROBLEMS HERE !! 
                printf("\nParent connection : /!\\ CONSTRUCTION ERROR /!\\\n\n");
            }
        }
        else{
            printf("This tile already has a connected tile beyond its %d-th edge\n", i);
            printf("Transition rules : GRTS[%d][%d] = %d\n", c->state, i, transition_rules(*c, i));
            if(range > 0){
                if(transition_rules(*c, i) > 0){
                    printf("Connected tile is in child state! Evaluating it...\n\n");
                    Generate_RTS(c->move[i], range - 1);
                }
                else{
                    printf("Connected tile is either a parent or a non-tree edge. Skipping it.\n\n");
                }
            }
            else{
                printf("Next tile out of bounds.\n");
            }
        }
    }
    printf("Done evaluating this cell.\n\n");
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

// ----- !! -----


int main(int argc, char** argv){
	assert(argc == 3);

	int type = atoi(argv[1]);
	int range = atoi(argv[2]);

	printf("\nType : %d \nRange : %d \n\n", type, range);

	assert(type == 5 || type == 4 || type == 7);
    assert(range > 0);

    // clock_t begin = clock();
	
	cell* g0 = NewTile(type, 0);
	
	Generate_RTS(g0, range - 1);

    // clock_t end = clock();
    // double duration = (double)(end - begin);

    printf("Successfully generated the RTS\n");
	
	print_RTS(g0, 0);
    // printf("RTS Generated in %.2f ms\n", duration);
	printf("\n\n");

	free_tile(g0);
	
	return 0;
}