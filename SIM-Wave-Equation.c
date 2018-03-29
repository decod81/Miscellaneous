/*
 * Mac: gcc SIM-Wave-Equation.c -framework Cocoa -lSDLmain -lSDL -lm
 * Linux: gcc SIM-Wave-Equation.c -lSDL -lm
 */
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <SDL/SDL.h>

int main() {
	SDL_Event e;
	double alpha, B[640][480], dB[640][480], t = 0.0;
	double complex P;
	int x, y, z, i, j, k, n;
	int X = 640, Y = 480;
	int px, py;
	char filename[80];
	int c;
	SDL_Surface *screen;
	Uint32* pixels;
	screen = SDL_SetVideoMode(X, Y, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
	pixels = (Uint32*)screen->pixels;

	px = rand()%600+20;
	py = rand()%400+20;

	// zero B and dB
	for(y=0; y<Y; y++)
		for(x=0; x<X; x++) {
			dB[x][y] = 0.;
			 B[x][y] = 0.;
		}

	while(e.type!=SDL_QUIT) {
		for(y=0; y<Y; y++) // dB = dB + d^2B/dx^2
			for(x=1; x<X-1; x++)
				dB[x][y] += (B[x+1][y] - 2*B[x][y] + B[x-1][y])/3.;

		for(x=1; x<X-1; x++) // dB = dB + d^2B/dy^2
			for(y=1; y<Y-1; y++)
				dB[x][y] += (B[x][y+1] - 2*B[x][y] + B[x][y-1])/3.;

		// terminating boundary conditions
		for(y=0; y<Y; y++) {
			 B[1][y]   =   B[2][y];
			dB[1][y]   = -dB[2][y];
			 B[X-1][y] =   B[X-2][y];
			dB[X-1][y] = -dB[X-2][y];
		}
		for(x=0; x<X; x++) {
			 B[x][1]   =   B[x][2];
			dB[x][1]   = -dB[x][2];
			 B[x][Y-1] =   B[x][Y-2];
			dB[x][Y-1] = -dB[x][Y-2];
		}
		for(y=0; y<Y; y++) // B = B + dB
			for(x=0; x<X; x++)
				B[x][y] += dB[x][y];

		alpha = 2*(sin(t/17.)+sin(t/11.));
		for(x=-30; x<30+1; x++)
			for(y=-6; y<6+1; y++) {
				P = (x + y*I)*cexp(I*alpha);
				B[(int)(X/2.+0.6*X/2.*sin(t/23.)+creal(P))][(int)(Y/2.+0.6*Y/2.*cos(t/17.)+cimag(P))] = 1.;
				dB[(int)(X/2.+0.6*X/2.*sin(t/23.)+creal(P))][(int)(Y/2.+0.6*Y/2.*cos(t/17.)+cimag(P))] = -.008;
			}
		for(y=0; y<Y; y++)
			for(x=0; x<X; x++) {
				c = 255.*B[x][y]*1.1;
				if(c>255) c=255;
				if(c<-255) c=-255;
#ifdef __APPLE__
				if(c>0) *(pixels+((int)y)*X+x) = 256*c;
				else *(pixels+((int)y)*X+x) = 256*65536*((int)(-c));
#else
				if(c>0) *(pixels+((int)y)*X+x) = c;
				else *(pixels+((int)y)*X+x) = 65536*((int)(-c));
#endif
			}

		if(rand()%255 == 1) {
			px = rand()%600+20;
			py = rand()%400+20;
		}

		for(y=0; y<11; y++)
			for(x=0; x<11; x++) {
				 B[px+x][py+y] = 0;
				dB[px+x][py+y] = 0;
#ifdef __APPLE__
				*(pixels+((int)(py+y))*X+px+x) = 256*(255 + 256*255 + 65536*255);
#else
				*(pixels+((int)(py+y))*X+px+x) = 255 + 256*255 + 65536*255;
#endif
			}

		t = t + 0.5;
		SDL_Flip(screen);
		SDL_PollEvent(&e);
		// sprintf(filename, "bmp/%05d.bmp", (int)(2*t));
		// SDL_SaveBMP(screen, filename);
		// ffmpeg -r 60 -f image2 -s 1920x1080 -i %05d.bmp -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4
		// ffmpeg -r 30 -f image2 -pattern_type glob -i 'bmp/*.bmp' -vcodec libx264 -crf 25 -pix_fmt yuv420p test2.mp4
		// if(t>500) exit(0);
	}
	return 0;
}
