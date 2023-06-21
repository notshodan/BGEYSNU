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

float dist(point, point);

point** line(point, point);

point* klein_proj(point);

point* poincare_proj(point);

point** geodesic_approx(point, point);