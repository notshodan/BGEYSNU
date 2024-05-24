// Unstable Unreliable Hyperbolic Engine
// Copyright (C) 2023-2024 Ellyas-Émile MARTINET

#include "transforms.h"

void rotation(double angle, int i, int j, double* v){
    assert(i != 3);
    assert(j != 3);
	assert((i >= 0) && (i < 3));
	assert((j >= 0) && (j < 3));

    double p[3]; p[0] = v[0]; p[1] = v[1]; p[2] = v[2];
    
    v[i] = p[i] * cos(angle) + p[j] * sin(angle);
	v[j] = p[j] * cos(angle) - p[i] * sin(angle);
}

void lboost(double angle, int i, double* v){
	// 1 -> x, 2 -> y, 3 -> z
	assert(i != 3);
	assert((i >= 0) && (i < 3));

	double p[3]; p[0] = v[0]; p[1] = v[1]; p[2] = v[2];

	v[i] = p[i] * cosh(angle) + p[2] * sinh(angle);
	v[2] = p[2] * cosh(angle) + p[i] * sinh(angle);
}

double center_to_edge_midpoint_distance(cell t){
    int k;
    switch(t.type){
        case 4:
            k = 5;
            break;
        case 5:
            k = 4;
            break;
        case 7:
            k = 3;
            break;
        default :
            exit(1);
            break;
    }
    return acosh(cos(M_PI / k)/sin(M_PI / t.type));
}

double center_to_vertices_distance(cell t){
    int k;
    switch(t.type){
        case 4:
            k = 5;
            break;
        case 5:
            k = 4;
            break;
        case 7:
            k = 3;
            break;
        default :
            exit(1);
            break;
    }
    return acosh((1/tan(M_PI / t.type)) * (1/tan(M_PI / k)));
}

double minkowski_InnerProduct(double* a, double* b){
    return a[0] * b[0] + a[1] * b[1] - a[2] * b[2];
}

/**
 * \fn void tile_relative_coordinates_transform(cell* t1, cell* t2, point p)
 * \brief Turns t1-relative coordinates for p into t2-relative coordinates. 
 * \warning Be aware that t2 must be a parent of t1 in the combinatorial map used for the tesselation, else this function may not end.
 * 
 * \param t1 The tile relative to which p's coordinates are
 * \param t2 The tile relative to which p's coordinates are going to be translated to
 * 
 * \return None. Modifies the values of p directly
*/
void tile_relative_coordinates_transform(cell* t1, cell* t2, double* p){
    //printf("*\n");
    cell* t = t1;
    int n = t->type;
    double distance = 2 * center_to_edge_midpoint_distance(*t1);
    //double distance = 1.022;

    while(t != t2){
        int j = t->spin[0];
        rotation(M_PI, 0, 1, p);
        lboost(distance, 0, p);
        rotation(- (TAU * j / n), 0, 1, p);
        t = t->move[0];
    }
}