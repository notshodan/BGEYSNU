// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

// This file defines common data structures, values and includes all necessary libraries

#ifndef _COMMONS_H_
#define _COMMONS_H_

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAU (2 * M_PI)
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 1080
#define RADIUS 0.003
#define CAMERA_RADIUS 20

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((b) < (a)) ? (a) : (b))

typedef double* point;

typedef struct cell_s {
    int type;                // number of neighbours
    int state;
    struct cell_s** move;    // list of neighbours
    int* spin;               // connections -- these last two fields mean that (g,i) connects to (g.move[i], g.spin[i])
    int content;             // -1 - Bomb, 0 and over - Info
    bool dug;
    bool flagged;
    SDL_Point center;
} cell;

typedef struct walker_s {
    cell* tile;
    int edge;
} walker;

typedef struct {
    int n;                  // number of tiles in the vector
    cell** c;               // tiles array
} vector;

typedef struct {
    cell* c;                // current centered cell
    double* c_position;     // camera position
} Camera;

typedef struct {
    int x;                  // x position
    int y;                  // y position
    cell* c;                // cell currently pointed at
    vector* v;              // array of all cells in the tiling
} Mouse;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
    SDL_Texture* entities[13];
    SDL_Surface* surface;
    SDL_Texture* texture;
} App;

extern Mouse mouse;

extern Camera camera;

extern App app;

extern int TESSELATION; // 0 = Poincaré, 1 = Klein

#endif