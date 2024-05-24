// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#ifndef _RENDER_H_
#define _RENDER_H_

#include "commons.h"
#include "controls.h"
#include "tesselation.h"
#include "transforms.h"
#include "vectors.h"

void cleanup(void);

void DrawEdge(double*, double*);

void add_to_render_queue(cell*, cell*);

SDL_Point flt_to_int_coordinates(double, double);

void prepareScene(int);

void presentScene(void);

void blit(vector*);

SDL_Texture* loadTexture(char*);

SDL_Texture* textureTable(int);

void initSDL(void);

#endif