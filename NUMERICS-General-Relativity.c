/* Draft-code of numerical methods for General Relativity
 */
#define N 20

double g[4][4][N*N*N*N]; /* spacetime metric tensors */
double T[4][4][N*N*N*N]; /* spacetime stress-energy tensors */

int npow(int i) {
	int val = 1;

	for(int j=0; j<i; j++)
		val = val*N;

	return val;
}

/* computes values of individual Christoffel symbols */
double christoffel(int i, int j, int l, int loc) {
	double val = 0.0, d;

	for(int k=0; k<4; k++) {
		d = \
			(g[k][i][loc + npow(j)] - g[k][i][loc - npow(j)]) +  \
			(g[k][j][loc + npow(i)] - g[k][j][loc - npow(i)]) -  \
			(g[i][j][loc + npow(k)] - g[i][j][loc - npow(k)]);
		val = val + g[l][k][loc]*d;
	}

	return 0.5*val;
}

/* computes values of individual components of Ricci curvature tensor */
double ricci(int i, int j, int loc) {
	double val = 0.0, a = 0.0, b = 0.0;

	for(int l=0; l<4; l++) val += \
		christoffel(i, j, l, loc + npow(l)) - christoffel(i, j, l, loc - npow(l));
	for(int l=0; l<4; l++) val -= \
		christoffel(i, l, l, loc + npow(j)) - christoffel(i, j, l, loc - npow(j));
	for(int m=0; m<4; m++)
		for(int l=0; l<4; l++) val += \
			christoffel(i, j, m, loc)*christoffel(m, l, l, loc);
	for(int m=0; m<4; m++)
		for(int l=0; l<4; l++) val -= \
			christoffel(i, l, m, loc)*christoffel(m, j, l, loc);

	return val;
}

/* computes scalar curvature */
double scalar_curvature(int i, int j, int loc) {
	double gi[4][4], A[4][4], val = 0.0;
	double A2[4][4], A3[4][4], trA1, trA2, trA3, det = 0.0;

	/* find metric tensor and initialize gi to zero */
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++) {
			A[i][j] = g[i][j][loc];
			gi[i][j] = 0.0;
		}

	/* compute g^2 and g^3 */
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				A2[i][j] += A[i][k]*A[k][j];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				A3[i][j] += A2[i][k]*A[k][j];

	/* use Cayley-Hamilton method to compute inverse of g */
	trA1 =  A[0][0] +  A[1][1] +  A[2][2] +  A[3][3];
	trA2 = A2[0][0] + A2[1][1] + A2[2][2] + A2[3][3];
	trA3 = A3[0][0] + A3[1][1] + A3[2][2] + A3[3][3];
	for(int i=0; i<4; i++) {
		gi[i][i] = (trA1*trA1*trA1 - 3*trA1*trA2 + 2*trA3)/6.0;
		for(int j=0; j<4; j++)
			gi[i][j] += A2[i][j]*trA1 - A3[i][j] - 0.5*A[i][j]*(trA1*trA1-trA2);
	}

	/* division of gi such that (g*gi)_{00} = 1 */
	for(int k=0; k<4; k++)
		det += A[0][k]*gi[k][0];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			gi[i][j] /= det;

	/* compute Ricci scalar */
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			val += gi[i][j]*ricci(i, j, loc);

	return val;
}

int main() {
}
