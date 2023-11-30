// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 minsieurmoi
// Contact : Discord - _yopedro ; Mail - minsieurmoi@gmail.com

// This file defines the functions used to create and update the tesselation 

#ifndef _TESSELATION_H_
#define _TESSELATION_H_

#include "commons.h"
#include "transforms.h"

int transition_rules(cell, int);

cell* NewTile(int, int);

void incr_walker(walker*, int);

walker* init_walker(cell*, int);

void Generate_RTS(cell*, int);

void step_walker(walker*);

void free_tile(cell*);

void connect7_1(walker*);

void connect71(walker*);

void print_RTS(cell*, int);

#endif