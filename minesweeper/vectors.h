#ifndef _VECTORS_H_
#define _VECTORS_H_

#include "commons.h"
#include "tesselation.h"

void fillVect(vector*, cell*, int*);

void freeVect(vector*);

vector* initVector(int, cell*);

int TileCount(cell*);

#endif