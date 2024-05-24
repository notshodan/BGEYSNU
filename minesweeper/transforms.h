// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

// This file defines all the transformations functions

#ifndef _TRANSFORMS_H_
#define _TRANSFORMS_H_

#include "commons.h"
#include "tesselation.h"

void rotation(double, int, int, double*);

void lboost(double, int, double*);

double center_to_edge_midpoint_distance(cell);

double center_to_vertices_distance(cell);

double minkowski_InnerProduct(double*, double*);

void tile_relative_coordinates_transform(cell*, cell*, double*);

#endif