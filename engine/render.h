// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 minsieurmoi
// Contact : Discord - _yopedro ; Mail - minsieurmoi@gmail.com

#ifndef _RENDER_H_
#define _RENDER_H_

#include "commons.h"
#include "controls.h"
#include "tesselation.h"
#include "transforms.h"

void cleanup(void);
SDL_Point flt_to_int_coordinates(double, double);
void initTesselation(cell*, int);
void prepareScene(cell*);
void presentScene(void);
void initSDL(void);

#endif