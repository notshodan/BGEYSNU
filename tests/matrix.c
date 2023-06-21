#include "librairies.h"

matrix* init_matrix(int l, int c){
	matrix* m = malloc(sizeof(matrix));
	m->lines = l;
	m->columns = c;
	double** content = malloc(sizeof(double) * l);
	for(int i = 0 ; i < c ; i++){
		content[i] = malloc(sizeof(double) * c);
	}
	m->mat = content;
	return m;
}

matrix* matrix_product(matrix A, matrix B){
	int l = A.lines;
	int c = B.columns;
	assert(l = c);
	matrix* m = init_matrix(l,c);
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			int val = 0;
			for(int k = 0; k < l; k++){
				val += A.mat[i][k] * B.mat[k][j];
			}
		m->mat[i][j] = val;
		}
	}
	return m;
}

void scalar(double coef, matrix* m){
	for(int i = 0; i < m->lines; i++){
		for(int j = 0; j < m->columns; j++){
			m->mat[i][j] = m->mat[i][j] * coef;
		}
	}
}

matrix* identity(int n){
	matrix* m = init_matrix(n , n);
	for(int i = 0; i < n; i++){
		m->mat[i][i] = 1;
	}
	return m;
}

void free_matrix(matrix* A){
	for(int i = 0 ; i < A->lines; i++){
		free(A->mat[i]);
	}
	free(A->mat);
	free(A);
}

void display_matrix(matrix A){
	for(int i = 0; i < A.lines; i++){
		for(int j = 0; j < A.columns; j++){
			printf("%.2f  ", A.mat[i][j]);
		}
		printf("\n");
	}
}
