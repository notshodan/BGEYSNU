#include <stdio.h>
#include <math.h>

#define DELTA 0.01
#define MIN -4.0
#define MAX 4.0

// hyperboloid : x² + y² - z² = -1 ie the upper sheet of the hyperboloid has equation z = sqrt(x² + y² + 1)
// disks below at z = 1 and z = 0 and radius of 1; a = (xa,ya,za) is in disk if xa² + ya² == 1 && za == 1
// let a = (xa,ya,za), then its vector to origin in equal to its coordinates
// ie (xa*x + ya*y)/za = z is the equation to the origin; we just have to take the coordinate for z = 1.
// the projected point is therefore a_p = (xa/za, ya/za, 1)

typedef double point[3]; // tried it with structs, its easier that way. x->0, y->1, z->2

struct pt_arr_s {
	point* arr;
	int len;
};

typedef struct pt_arr_s pt_arr;


int quick_pow(double x, int exp){
	if(exp == 0){
		return 1;
	}
	if(exp == 1){
		return x;
	}
	if(exp % 2 == 0){
		return quick_pow(x * x, exp / 2);
	} else {
		return x * quick_pow(x * x, (exp - 1) / 2);
	}
}

double Q_rsqrt(double nb){
	long i;
	double x2, y;
	const double threehalfs = 1.5F;

	x2 = nb * 0.5F;
	y = nb;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 ); 
	y = * ( double * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) ); 
	y = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

double FBS(point a, point b){
	double fbs;
	fbs = (a[0] * b[0]) - (a[1] * b[1]) - (a[2] - b[2]);
	return fbs;
}

double PDN(point a){
	return sqrt(FBS(a, a));
}

double distance(point a, point b){
	double nx = (b[0] - a[0]);
	double ny = (b[1] - a[1]);  
	double nz = (b[2] - a[2]);
	return sqrt(nx * nx + ny * ny + nz * nz);
}

double flt_abs(double x){
	double abs = x > 0 ? x : -x;
	return abs;
}

pt_arr* init_line(point a, point b){
	double dist = distance(a,b);
	int nb_interpolated_pts = (int) dist * 50;
	point* line = malloc(sizeof(point) * nb_interpolated_pts); 
	double dx = (b.x - a.x) / nb_interpolated_pts;
	double dy = (b.y - a.y) / nb_interpolated_pts;

	for(int i = 0; i < nb_interpolated_pts; i++){
		point p = malloc(sizeof(double) * 3);
		p[0] = a[0] + i * dx;
		p[1] = a[1] + i * dy;
		p[2] = 1 ; // we'll consider the point a and b are from the klein projection
		line[i] = p;
	}
	pt_arr* res = malloc(sizeof(pt_arr));
	res->arr = line;
	res->len = nb_interpolated_pts;
	return res;
}

void free_pt_arr(pt_arr* ln){
	for(int i = 0; i < ln->len; i++){
		free(ln->arr[i]);
	}
	free(ln->arr);
	free(ln);
}

point klein_proj(point a){
	point p = malloc(sizeof(double) * 3);
	p[0] = a[0] / a[2];
	p[1] = a[1] / a[2];
	p[2] = 1;
	return p;
}

point poincare_proj(point a){
	point p = malloc(sizeof(double) * 3);
	p[0] = a[0] / (a[2] + 1);
	p[1] = a[1] / (a[2] + 1);
	p[2] = 0;
	return p;
}

/* 
pt_arr* geodesic_approx(point a, point b){ // here suppose a and b are on the hyperboloid
	point ka = klein_proj(a);
	point kb = klein_proj(b);
	pt_arr* line = init_line(*ka, *kb);
	pt_arr* geodesic = malloc(sizeof(pt_arr));
	geodesic->len = line->len;
	geodesic->arr = malloc(sizeof(point*) * (line->len));
	for(int i = 0; line->len; i++){
		point pt_i = *(line->arr[i]);
		double t = Q_rsqrt(1 - pt_i.x*pt_i.x - pt_i.y*pt_i.y);
		point* p = malloc(sizeof(point));
		p->x = pt_i.x * t;
		p->y = pt_i.y * t;
		p->z = t;
	}
	free_pt_arr(line);
	free(ka);
	free(kb);
	return geodesic;
}
not sure i'll need this code at all but anyways i'll have to redo it 
*/

double sinh(double x){
	return (exp(x) - exp(-x))/2;
}

double cosh(double x){
	return (exp(x) + exp(-x))/2;
}

point lboost(double angle, int i, int j, point v){
	// 1 -> x, 2 -> y, 3 -> z
	assert(i != j);
	assert((i >= 0) && (i < 3));
	assert((j >= 0) && (j < 3));
	int k = 6 - i - j;
	point new_v = (point) malloc(sizeof(double) * 3);
	new_v[i] = v[i] * cosh(angle) + v[j] * sinh(angle);
	new_v[j] = v[j] * cosh(angle) + v[i] * sinh(angle);
	new_v[k] = v[k];
	return new_v;
}

double inr_prdh(point a, point b){
	double mk;
	mk = (a[0] * b[0]) + (a[1] * b[1]) - (a[2] * b[2])
	return mk;
}

double arcosh(double x){
	double res;
	res = log(x + sqrt((x*x) - 1));
/*
struct tile_s {
	pt_arr* edges;
	int val;
	struct tile_s** chdn;
};

typedef struct tile_s tile;

int nb_chdn(tile, int);
*/
	return res;
}

double disth(point x, point y){
	double res;
	res = arcosh(inr_prdh(x, y));
	return res;
}

/*
struct tile_s {
	pt_arr* edges;
	int val;
	struct tile_s** chdn;
};

typedef struct tile_s tile;

int nb_chdn(tile, int);
*/

struct tile_s {
	int type;
	struct tile_s* move[15];
	int spin[15];
};

typedef tile_s tile;
