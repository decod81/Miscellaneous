// compile with -lSDL2main -lSDL2
// ffmpeg -f image2 -r 30 -s 1280x720 -pix_fmt gray -vcodec rawvideo -i %05d.raw -pix_fmt yuv420p -vcodec libx264 -crf 2 output.mp4
// gray16le

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int WinMain(int argc, char* args[]) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Surface *surface;
    int X = 640, Y = 360;
    int *p, x, y, n, c = 1;
	long long counter=0;
	float x0, y0;
	unsigned char *plane = malloc(X*Y);
	char filename[128];
	float complex z, z0;
	FILE *f;

	for(n=0; n<X*Y; n++)
		plane[n] = 0;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, X, Y, 0);
    surface = SDL_GetWindowSurface(window);

    while(1) {
		 x = rand()%32767;
		 y = rand()%32767;
		x0 = 1.5*x/(32767/4.0) - 3.0;
		y0 = 1.0*y/(32767/4.0) - 2.0;
		z0 = x0 + y0*I;
		 z = 0;
		for(n=0; n<20; n++) /**** z = z^2 + z0 ****/
			z = z*z + z0;
		if(cabs(z)>4) {
		// if(sqrt(creal(z)*creal(z)+cimag(z)*cimag(z))>4) {
			z = 0.0;
			for(n=0; n<25; n++) {
				z = z*z + z0;
				x = X/1.8 + 3.0*X/16*creal(z);
				y = Y/2.0 + 3.0*Y/9*cimag(z);
				if( (x>-1) && (x<X) && (y>-1) && (y<Y) && plane[x + y*X]<255 )
					plane[x + y*X]++;
			}
		}
		if(!(++counter%500000)) {
			// sprintf(filename, "%05d.raw", c++);
			// f = fopen(filename, "wb");
			for(n=0; n<X*Y; n++) {
				p = (int *)surface->pixels+n;
                *p = 65536*plane[n] + 256*plane[n] + plane[n];
				// fprintf(f, "%c", (unsigned char)(plane[n]));
			}
			// fwrite(plane, 1, X*Y, f);
			// fflush(f);
			// fclose(f);
			SDL_UpdateWindowSurface(window);
		}
        if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
    }
    free(plane);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
