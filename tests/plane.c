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

struct point_s {
	float x;
	float y;
	float z;
};

typedef struct point_s point;

struct pt_arr_s {
	point** arr;
	int len;
};

typedef struct pt_arr_s pt_arr;


int quick_pow(float x, int exp){
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

float Q_rsqrt(float nb){
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = nb * 0.5F;
	y = nb;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 ); 
	y = * ( float * ) &i;
	y = y * ( threehalfs - ( x2 * y * y ) ); 
	y = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

float FBS(point a, point b){
	float fbs;
	fbs = (a.x * b.x) - (a.y * b.y) - (a.z - b.z);
	return fbs;
}

float PDN(point a){
	return sqrt(FBS(a, a));
}

float distance(point a, point b){
	float nx = (b.x - a.x)*(b.x - a.x);
	float ny = (b.y - a.y)*(b.y - a.y);  
	float nz = (b.z - a.z)*(b.z - a.z);
	return sqrt(nx + ny + nz);
}

float flt_abs(float x){
	float abs = x > 0 ? x : -x;
	return abs;
}

pt_arr* init_line(point a, point b){
	float dist = distance(a,b);
	int nb_interpolated_pts = dist * 50;
	point** line = malloc(sizeof(point*) * nb_interpolated_pts); 
	float dx = (b.x - a.x) / nb_interpolated_pts;
	float dy = (b.y - a.y) / nb_interpolated_pts;

	for(int i = 0; i < nb_interpolated_pts; i++){
		point* p = malloc(sizeof(point));
		p->x = a.x + i * dx;
		p->y = a.y + i * dy;
		p->z = 1 ; // we'll consider the point a and b are from the klein projection
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

point* klein_proj(point a){
	point* p = malloc(sizeof(point));
	p->x = a.x / a.z;
	p->y = a.y / a.z;
	p->z = 1;
	return p;
}

point* poincare_proj(point a){
	point* p = malloc(sizeof(point));
	p->x = a.x / (a.z + 1);
	p->y = a.y / (a.z + 1);
	p->z = 0;
	return p;
}

pt_arr* geodesic_approx(point a, point b){ // here suppose a and b are on the hyperboloid
	point* ka = klein_proj(a);
	point* kb = klein_proj(b);
	pt_arr* line = init_line(*ka, *kb);
	pt_arr* geodesic = malloc(sizeof(pt_arr));
	geodesic->len = line->len;
	geodesic->arr = malloc(sizeof(point*) * (line->len));
	for(int i = 0; line->len; i++){
		point pt_i = *(line->arr[i]);
		float t = Q_rsqrt(1 - pt_i.x*pt_i.x - pt_i.y*pt_i.y);
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

double sinh(double x){
	return (exp(x) - exp(-x))/2;
}

double cosh(double x){
	return (exp(x) + exp(-x))/2;
}

point* lboost(float angle, int i, int j, point v){
	// 1 -> x, 2 -> y, 3 -> z
	assert(i != j);
	int k = 6 - i - j;
	point* rv = malloc(sizeof(point));
	double[3] pt;
	double[3] tmp;
	tmp[0] = v.x;
	tmp[1] = v.y;
	tmp[2] = v.z;
	pt[i] = tmp[i] * cosh(a) + tmp[j] * sinh(a);
	pt[j] = tmp[j] * cosh(a) + tmp[i] * sinh(a);
	//...
	return rv;
}

struct tile_s {
	pt_arr* edges;
	int val;
	struct tile_s** chdn;
};

typedef struct tile_s tile;

int nb_chdn(tile, int);