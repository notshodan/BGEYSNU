#include "librairies.h"
#include "matrix.h"
#include "matrix.c"

void rotation(matrix* point, matrix axis, double a){
	// rotates a given point around a given axis on the unit sphere by a give angle

	matrix* r = identity(3);

	// the axis is a vector, which should be normalized first (ie x² + y² + z² = 1)
	const double u = axis.mat[0][0];
	const double v = axis.mat[1][0];
	const double w = axis.mat[2][0];
	
	// could use a taylor-young style approximation, as this is going to yield
	// the same values as if a was in the [-2pi; 2pi] interval
	const double c = cos(a);
	const double s = sin(a):

	// creation of the rotation matrix
	r->mat[0][0] = c + pow(u, 2) * (1 - c);
	r->mat[0][1] = u * v * (1 - c) - w * s;
	r->mat[0][2] = u * w * (1 - c) + v * s;
	r->mat[1][0] = u * v * (1 - c) + w * s;
	r->mat[1][1] = c + pow(v, 2) * (1 - c);
	r->mat[1][2] = w * v * (1 - c) - u * s;
	r->mat[2][0] = u * w * (1 - c) - v * s;
	r->mat[2][1] = w * v * (1 - c) + u * s;
	r->mat[2][2] = c + pow(w, 2) * (1 - c);

	matrix* point_tmp = matrix_product(*r, *point);
	*point = *point_tmp;

	free_matrix(point_tmp);
	free_matrix(r);
}
