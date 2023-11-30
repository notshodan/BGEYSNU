// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 minsieurmoi
// Contact : Discord - _yopedro ; Mail - minsieurmoi@gmail.com

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAU (2 * M_PI)
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define RADIUS 0.75

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((b) < (a)) ? (a) : (b))

typedef double* point;

typedef struct matrix_s {
	double** mat;
	int lines;
	int columns;
} matrix;

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

typedef struct {
    cell* c;
    double* c_position;
} Camera;

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
} App;


extern Camera camera;

extern App app;

#endif