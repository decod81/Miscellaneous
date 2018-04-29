#include <stdio.h>

#define UINT_MAX 4294967295
#define INT_MAX  2147483647
#define RAND_MAX 4294967295

#define M_PI 3.14159265358979323846264338327950288419716939937508

/**** standard I/O and string handling ****/
/* SFS (simple file system, 512 byte blocks): End includes a list of files in reverse order  (NAME64, OFFSET32, SIZE32) */
/*
  fopen
  fread
  fwrite
  fseek
  ftell
  rewind
  fclose 
  fprintf
*/
/**** standard I/O and string handling ****/

/**** Mersenne Twister with a period of 2^19937 - 1 ****/
#define N              (624)
#define M              (397)
#define K              (0x9908B0DFU)
#define hiBit(u)       ((u) & 0x80000000U)
#define loBit(u)       ((u) & 0x00000001U)
#define loBits(u)      ((u) & 0x7FFFFFFFU)
#define mixBits(u, v)  (hiBit(u)|loBits(v))

unsigned int state[N+1];
unsigned int *next;
int left = -1;

seedMT(unsigned int seed) {
  unsigned int x = (seed | 1U) & 0xFFFFFFFFU, *s = state;
  int j;

  for(left=0, *s++=x, j=N; --j; *s++ = (x*=69069U) & 0xFFFFFFFFU);
}

unsigned int reloadMT() {
  unsigned int *p0=state, *p2=state+2, *pM=state+M, s0, s1;
  int j;

  if(left<-1)
    seedMT(4357U);

  left = N-1;
  next = state+1;

  for(s0=state[0], s1=state[1], j=N-M+1; --j; s0=s1, s1=*p2++)
    *p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);

  for(pM=state, j=M; --j; s0=s1, s1=*p2++)
    *p0++ = *pM++ ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);

  s1 = state[0];
  *p0 = *pM ^ (mixBits(s0, s1) >> 1) ^ (loBit(s1) ? K : 0U);
  s1 ^= (s1 >> 11);
  s1 ^= (s1 <<  7) & 0x9D2C5680U;
  s1 ^= (s1 << 15) & 0xEFC60000U;
  return (s1 ^ (s1 >> 18));
}

unsigned int rand() {
  unsigned int y;

  if(--left < 0)
    return (reloadMT());

  y  = *next++;
  y ^= (y >> 11);
  y ^= (y <<  7) & 0x9D2C5680U;
  y ^= (y << 15) & 0xEFC60000U;
  return (y ^ (y >> 18));
}
/**** Mersenne Twister ****/

/**** Mathematics ****/
struct complex {
  double real;
  double imag;
};

int alt_abs(int a) {
  if(a<0) return -a;
  else return a;
}

/* iteratively invert a growing function with a and b as limits */
double alt_inv(double (*f)(), double x, double a, double b) {
  int i, iterations = 30;
  
  for(i=0; i<30; i++)
    if(f((a+b)/2)<x)
	  a = (a+b)/2;
    else
	  b = (a+b)/2;
    
  return (a+b)/2;
}

/* c = a * b */
cmul(struct complex a, struct complex b, struct complex *c) {
  struct complex A;
  struct complex B;
  A.real = a.real;
  A.imag = a.imag;
  B.real = b.real;
  B.imag = b.imag;
  c->real = A.real*B.real - A.imag*B.imag;
  c->imag = A.imag*B.real + A.real*B.imag;
}

/* v = exp(z) */
alt_cexp(struct complex z, struct complex *v) {
  int k;
  struct complex w;
  struct complex x;
  v->real = 1; v->imag = 0;
  w.real = 1; w.imag = 0;
  for(k=1; k<20; k++) {
    x.real = z.real/k;
    x.imag = z.imag/k;
	cmul(w, x, &w);
    v->real = v->real + w.real;
    v->imag = v->imag + w.imag;
  }
}

/* square */
double alt_sqr(double x) {
  return x*x;
}

/* square root */
double alt_sqrt(double x) {
  if(x>1)
    return alt_inv(alt_sqr, x, 1, x);
  else
    return alt_inv(alt_sqr, x, x, 1);
}

double alt_exp(double x) {
  struct complex X;
  struct complex z;
  X.real = x;
  alt_cexp(X, &z);
  return z.real;
}

double alt_log(double x) {
  if(x<1)
    return -alt_log(1/x);
  else
    return alt_inv(alt_exp, x, 0, x);
}

/* trigonometric functions */
double alt_sin(double x) {
  struct complex X;
  struct complex z;
  X.imag = x;
  alt_cexp(X, &z);
  return z.imag;
}
double alt_cos(double x) {
  struct complex X;
  struct complex z;
  X.imag = x;
  alt_cexp(X, &z);
  return z.real;
}
double alt_tan(double x) {
  return alt_sin(x)/alt_cos(x);
}
double alt_atan(double x) {
  if(x<0)
    return -alt_atan(-x);
  else
    return alt_inv(alt_tan, x, 0, 1.6);
}
double alt_asin(double x) {
  return alt_atan(x/alt_sqrt(1-x*x));
}
double alt_acos(double x) {
  return 2*alt_atan(1)-alt_atan(x/alt_sqrt(1-x*x));
}

double alt_pow(double x, double y) {
  return alt_exp(y*alt_log(x));
}

/* uniformly distributed doubles [0..1] */
double alt_rand() {
  return (rand()+1.0)/(RAND_MAX+1.0);
}

/* normally distributed doubles with standard deviation of 1 */
double alt_randn() {
  return alt_sqrt(-2*alt_log(alt_rand()))*cos(2*M_PI*alt_rand());
}
/**** Mathematics ****/

int main() {
  struct complex x, y, z;
  int i;

  seedMT(4357U); // best seeds are odd values up to 2^32-1
  for(i=0; i<8; i++)
    printf("%f\n", alt_rand());
  x.real = 1;
  alt_cexp(x, &z);
  printf("%f %f\n", z.real, z.imag);
}
