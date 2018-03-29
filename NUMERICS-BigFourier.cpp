/*
 * arbitrary precision complex numbers and Fourier transform
 * c++ NUMERICS-BigFourier.cpp -lmpfr
 */

#include <mpfr.h>
#include <iostream>
#include <fstream>
#include <string>
#include <complex>
#include <cmath>
using namespace std;

#define N 1000

struct bigfloat {
	mpfr_t value;

	bigfloat() {
		mpfr_init2(value, N);
	}
	bigfloat operator*(const bigfloat& rhs) { // bigfloat *
		bigfloat result;
		mpfr_mul(result.value, value, rhs.value, MPFR_RNDN);
		return result;
	}
	bigfloat operator/(const bigfloat& rhs) { // bigfloat /
		bigfloat result;
		mpfr_div(result.value, value, rhs.value, MPFR_RNDN);
		return result;
	}
	bigfloat operator/(const double& rhs) { // double /
		bigfloat result;
		mpfr_div_d(result.value, value, rhs, MPFR_RNDN);
		return result;
	}
	bigfloat operator+(const bigfloat& rhs) { // bigfloat +
		bigfloat result;
		mpfr_add(result.value, value, rhs.value, MPFR_RNDN);
		return result;
	}
	bigfloat operator-(const bigfloat& rhs) { // bigfloat -
		bigfloat result;
		mpfr_sub(result.value, value, rhs.value, MPFR_RNDN);
		return result;
	}
	bigfloat& operator=(const bigfloat& rhs) { // bigfloat =
		mpfr_set(value, rhs.value, MPFR_RNDN);
		return *this;
	}
	bigfloat& operator=(const double& rhs) { // double =
		mpfr_set_d(value, rhs, MPFR_RNDN);
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& stream, bigfloat& rhs) {
		char* str = NULL;

		// change the number of digits if you want
		mpfr_asprintf(&str, "%.10Rf", rhs.value);
		stream << str << "\n";
		mpfr_free_str(str);
	}
	~bigfloat() {
		mpfr_clear(value);
	}
};

struct bigcomplex {
	bigfloat real;
	bigfloat imag;

	bigcomplex() {
		real = 0.0;
		imag = 0.0;
	}
	bigcomplex operator+(struct bigcomplex& rhs) { // bigcomplex +
		bigcomplex result;
		result.real = real + rhs.real;
		result.imag = imag + rhs.imag;
		return result;
	}
	bigcomplex operator-(struct bigcomplex& rhs) { // bigcomplex -
		bigcomplex result;
		result.real = real - rhs.real;
		result.imag = imag - rhs.imag;
		return result;
	}
	bigcomplex operator*(const bigcomplex& rhs) { // bigcomplex *
		bigcomplex result;
		result.real = real*rhs.real - imag*rhs.imag;
		result.imag = imag*rhs.real + real*rhs.imag;
		return result;
	}
	bigcomplex& operator=(const bigcomplex& rhs) { // bigcomplex =
		real = rhs.real;
		imag = rhs.imag;
		return *this;
	}
	bigcomplex& operator=(const complex<double>& rhs) { // double complex =
		real = rhs.real();
		imag = rhs.imag();
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& stream, bigcomplex& rhs) {
		char* str = NULL;

		// change the number of digits if you want
		mpfr_asprintf(&str, "%.10Rf", rhs.real.value);
		stream << str;
		mpfr_free_str(str);
		stream << " + ";
		mpfr_asprintf(&str, "%.10Rf", rhs.imag.value);
		stream << str;
		mpfr_free_str(str);
		stream << "i\n";
	}
};

bigfloat PI;

int dft(bigcomplex a[], bigcomplex b[], int n) {
	bigcomplex p, q;
	bigfloat x, y;
	int m=0, t=0;

	for(int m=0; m<n; m++)
		for(int t=0; t<n; t++) {
			// b[m] = b[m] + a[t]*cexp(-I*2*PI*m*t/n);
			x = -2.0*m*t;
			x = x*PI/n;
			mpfr_cos(p.real.value, x.value, MPFR_RNDN);
			mpfr_sin(p.imag.value, x.value, MPFR_RNDN);
			q = a[t]*p;
			b[m] = b[m] + q;
	}
}

int main() {
	mpfr_t PIm;
	bigcomplex inp[4];
	bigcomplex out[4];

	mpfr_init2(PIm, N);
	mpfr_const_pi(PIm, MPFR_RNDN);
	mpfr_set(PI.value, PIm, MPFR_RNDN);

	int x = 0, y = 0;
	string line;
	ifstream myfile("data.txt");
	while(getline(myfile, line)) {
		if(x%2==0) mpfr_set_str(inp[y].real.value, line.c_str(), 10, MPFR_RNDN);
		else mpfr_set_str(inp[y++].imag.value, line.c_str(), 10, MPFR_RNDN);
		x++;
	}
	myfile.close();

	dft(inp, out, 4);

	for(int x=0; x<4; x++)
		cout << out[x];

	return 0;
}
