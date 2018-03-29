/* A numerical Navier-Stokes simulation, output saved into *.ppm
 *
 * Convert into gif-animation: convert -layers Optimize -delay 3 *.pgm navier-stokes.gif
 *
 * gcc navier-stokes.c -lm
 */
#include <stdio.h>
#include <math.h>

int main() {
	char filename[80];
	int N = 128, c = 1, x, y;
	float  f[128*128]; float  p[128*128];
	float  u[128*128]; float  v[128*128];
	float ux[128*128]; float vx[128*128];
	float uy[128*128]; float vy[128*128];
	float u2[128*128]; float v2[128*128];
	float us[128*128]; float vs[128*128];
	float px[128*128]; float py[128*128];
	for(x=0; x<128*128; x++) {
		 f[x] = 0.0;  p[x] = 0.0;
		 u[x] = 0.0;  v[x] = 0.0;
		ux[x] = 0.0; vx[x] = 0.0;
		uy[x] = 0.0; vy[x] = 0.0;
		u2[x] = 0.0; v2[x] = 0.0;
		us[x] = 0.0; vs[x] = 0.0;
		px[x] = 0.0; py[x] = 0.0;
	}
	for(int t=0; t<4500; t++) {
		for(y=-5; y<5; y++)
			for(x=-5; x<5; x++) {
				u[(int)(0.75*N+x)+(N/2+y)*N] = 0.15;
				v[(int)(0.75*N+x)+(N/2+y)*N] = 0.0;
			}
		for(y=1; y<N-1; y++)
			for(x=1; x<N-1; x++) {
				ux[x+y*N] = 0.50*(u[(x+1)+y*N] -                u[(x-1)+y*N]);
				vx[x+y*N] = 0.50*(v[(x+1)+y*N] -                v[(x-1)+y*N]);
				uy[x+y*N] = 0.50*(u[x+(y+1)*N] -                u[x+(y-1)*N]);
				vy[x+y*N] = 0.50*(v[x+(y+1)*N] -                v[x+(y-1)*N]);
				u2[x+y*N] = 0.25*(u[(x+1)+y*N] - 2.0*u[x+y*N] + u[(x-1)+y*N]);
				v2[x+y*N] = 0.25*(v[(x+1)+y*N] - 2.0*v[x+y*N] + v[(x-1)+y*N]);
				u2[x+y*N]+= 0.25*(u[x+(y+1)*N] - 2.0*u[x+y*N] + u[x+(y-1)*N]);
				v2[x+y*N]+= 0.25*(v[x+(y+1)*N] - 2.0*v[x+y*N] + v[x+(y-1)*N]);
			}
		for(x=0; x<N*N; x++) {
			us[x] = u[x] + u[x]*ux[x] + v[x]*uy[x] + u2[x]/20.0 - px[x]/20.0 + 0.00000;
			vs[x] = v[x] + u[x]*vx[x] + v[x]*vy[x] + v2[x]/20.0 - py[x]/20.0 - 0.00005;
		}
		for(y=1; y<N-1; y++)
			for(x=1; x<N-1; x++) {
				f[x+y*N] = 0.5*(us[(x+1)+y*N] - us[(x-1)+y*N] + vs[x+(y+1)*N] - vs[x+(y-1)*N]);
				p[x+y*N] = 0.25*(p[(x+1)+y*N] + p[x+(y+1)*N] + p[(x-1)+y*N] + p[x+(y-1)*N] - f[x+y*N]);
			}
		for(y=1; y<N-1; y++)
			for(x=1; x<N-1; x++) {
				px[x+y*N] = 0.5*(p[(x+1)+y*N] - p[(x-1)+y*N]);
				py[x+y*N] = 0.5*(p[x+(y+1)*N] - p[x+(y-1)*N]);
				u[x+y*N] = us[x+y*N] - px[x+y*N];
				v[x+y*N] = vs[x+y*N] - py[x+y*N];
			}
		if(t%40==0) {
			printf("%d\n", t);
			sprintf(filename, "%04d.pgm", c);
			FILE *f = fopen(filename, "w");
			fprintf(f, "P2\n%d %d\n%d\n", 128, 128, 255);
			c++;
			for(int j=0; j<128; j++) {
				for(int i=0; i<128; i++) {
					float val = 1500.0*sqrt( u[i+j*N]*u[i+j*N] + v[i+j*N]*v[i+j*N]);
					if(val>255.0) val=255.0;
					fprintf(f, "%3d ", (unsigned char)val);
				}
				fprintf(f, "\n");
			}
			fclose(f);
		}
	}
}
