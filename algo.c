#include <stdio.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846264338327950288419716939937508

/* multiplication without multiplication */
int alt_mul(int a, int b) {
  int result = 0, i, j = 0;
 
  if(b!=0)
    while(((b<<j)>>30)!=1)
      j++;
  for(i=0; i<16; i++)
    if((b>>i)&1)
      result = result + (a << i);
   
  return result;
}

/* division without division */
int alt_div(int a, int b) {
  int remainder = a, result = 0, i = 0, j;

  while(((b<<i)>>30)!=1)
    i++; 
  while(i>=0) {
    j = b<<i;
    if(remainder - j >= 0) {
      remainder = remainder - j;
      result = result + (1<<i);
    }
    else
      i = i - 1;
  }

  return result;
}

/* complex valued exp using Taylor series */
double complex alt_cexp(double complex z) {
  double complex v = 1, w = 1;
  for(int k=1; k<20; k++)
    v += (w*=z/k);
  return v;
}

/* iteratively invert a growing function with a and b as limits */
double inv(double (*f)(), double x, double a, double b) {
  for(int i=0; i<20; i++)
    if(f((a+b)/2)<x) a = (a+b)/2;
    else b = (a+b)/2;
  return (a+b)/2;
}

double alt_sqr(double x) {
  return x*x;
}

double alt_sqrt(double x) {
  if(x>1) return inv(alt_sqr, x, 1, x);
  return inv<(alt_sqr, x, x, 1);
}

double alt_exp(double x) {
  return creal(alt_cexp(x));
}

double alt_log(double x) {
  if(x<1) return -alt_log(1/x);
  return inv(alt_exp, x, 0, x);
}

double alt_sin(double x) { return cimag(alt_exp(I*x)); }
double alt_cos(double x) { return creal(alt_exp(I*x)); }
double alt_tan(double x) { return alt_sin(x)/alt_cos(x); }
double alt_cot(double x) { return alt_cos(x)/alt_sin(x); }
double alt_pow(double x, double y) { return creal(alt_exp(y*alt_log(x))); }
double alt_sinh(double x) { return (creal(alt_exp(x))-creal(alt_exp(-x)))/2; }
double alt_cosh(double x) { return (creal(alt_exp(x))+creal(alt_exp(-x)))/2; }
double alt_tanh(double x) { return alt_sinh(x)/alt_cosh(x); }
double alt_coth(double x) { return alt_cosh(x)/alt_sinh(x); }
double alt_log10(double x) { return alt_log(x)/alt_log(10); }
double alt_atan(double x) { if(x<0) return -alt_atan(-x); return inv(alt_tan, x, 0, 1.6); }
double alt_asin(double x) { return alt_atan(x/alt_sqrt(1-x*x)); }
double alt_acos(double x) { return 2*alt_atan(1)-alt_atan(x/alt_sqrt(1-x*x)); }

/* discrete Fourier transform: F(w) = sum(f(t)e^(-iwt)) */
int dft(double complex *a, double complex *b, int n) {
  for(int m=0; m<n; m++) {
    b[m] = 0;
    for(int t=0; t<n; t++) /* w = 2*PI*m/n */
      b[m] = b[m] + a[t]*cexp(-I*2*PI*m*t/n); /* (a+bi)*(c+di) */
  }
}

/* more efficient, less intuitive Fourier transform */
int fft(double complex a[], double complex b[], int n, int step) {
  double complex t;
  if(step==1) for(int i=0; i<n; i++) b[i] = a[i];
  if(step<n) {
    fft(b, a, n, step * 2);
    fft(b + step, a + step, n, step * 2);
    for(int i=0; i<n; i+=2*step) {
      t = cexp(-I*PI*i/n)*a[i+step];
      b[i/2] = a[i]+t;
      b[(i+n)/2] = a[i]-t;
    }
  }
} /* notice that this alters the input vector */

int main() {
  printf("%d %d\n", alt_mul(30003, 45), 30003*45);
  printf("%d %d\n", alt_div(30003, 45), 30003/45);
  printf("%f %f\n", creal(alt_cexp(1.0)), creal(cexp(1.0)));
}
