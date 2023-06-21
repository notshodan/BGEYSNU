#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "librairies.h"

struct matrix_s {
	double** mat;
	int lines;
	int columns;
};

typedef struct matrix_s matrix;

matrix* init_matrix(int, int);

matrix* matrix_product(matrix, matrix);

void free_matrix(matrix*);

void display_matrix(matrix);

#endif
